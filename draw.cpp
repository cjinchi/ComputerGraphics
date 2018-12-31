#include "draw.h"
#include <QVector>
#include <QRectF>
#include <QMessageBox>
#include "polygon_info.h"
#include <QImage>
#include <QCursor>
#include <QtGlobal>
#include <QtMath>

draw::draw(QWidget *parent) : QWidget(parent)
{
    current_global_state = NONE;
    current_dragging_state = INVALID;
    current_fill_state = FILL_WAITING;

    pixmap = QPixmap(WINDOW_WIDTH,WINDOW_HEIGHT);
    pixmap.fill(Qt::white);
    temp_pixmap = pixmap;
    temp_temp_pixmap = temp_pixmap;

    p1 = QPoint(0,0);
    p2 = QPoint(0,0);
    p1_backup = QPoint(0,0);
    p2_backup = QPoint(0,0);
    setRotate(180);
    setZoom(100);

    ellipse_xc = 0;
    ellipse_yc = 0;
    ellipse_rx = 0;
    ellipse_ry = 0;

    first_time_using_polygon = true;
    first_time_using_curve = true;

    setCursor(Qt::ArrowCursor);

    rotate =180;
    zoom = 100;

    currentColor = Qt::black;
    currentPenWidith = 2;
}

void draw::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    temp_pixmap = pixmap;
    QPainter painter;

    if(current_global_state == DRAG)
    {
        if(current_dragging_state == MOVING||current_dragging_state == DRAGGING || current_dragging_state == EDITING ||current_dragging_state==DECIDING)
        {
            painter.begin(&temp_pixmap);
            QPen pen = painter.pen();
            pen.setWidth(currentPenWidith);
            pen.setColor(currentColor);
            painter.setPen(pen);
            switch(current_shape)
            {
            case LINE:
                drawLine(painter);
                break;
            case CIRCLE:
                drawCircle(painter);
                break;
            case ELLIPSE:
                ellipse_xc = (p1.x()+p2.x())/2;
                ellipse_yc = (p1.y()+p2.y())/2;
                ellipse_rx = qAbs(p1.x()-p2.x())/2;
                ellipse_ry = qAbs(p1.y()-p2.y())/2;
                drawEllipse(painter);
                break;
            case POLYGON:
                drawPolygon(painter);
                break;
            case CURVE:
                drawCurve(painter);
                break;
            case CLIP:
                break;
            }
            painter.end();

            temp_temp_pixmap = temp_pixmap;
            painter.begin(&temp_temp_pixmap);

            painter.setPen(Qt::blue);
            if(current_shape == POLYGON || current_shape == CURVE)
            {
                for(int i=0;i<points.size();i++)
                {
                    draw_small_point(painter,points[i]);
                }
                if(current_dragging_state == MOVING || current_dragging_state == EDITING ||current_dragging_state==DECIDING)
                {
                    draw_small_rectangle(painter,center);
                }
            }
            else
            {
                draw_small_point(painter,p1);
                draw_small_point(painter,p2);
                draw_small_rectangle(painter,(p1+p2)/2);
                if(current_shape==ELLIPSE || current_shape == CLIP)
                {
                    painter.setPen(Qt::DotLine);
                    //此处是为了画出辅助线，非二维绘图算法实现，因此直接调用Qt框架API
                    if(current_shape == ELLIPSE && rotate ==180){
                        painter.drawRect(qMin(p1.x(),p2.x()),qMin(p1.y(),p2.y()),qAbs(p1.x()-p2.x()),qAbs(p1.y()-p2.y()));
                    }else if(current_shape == CLIP){
                        painter.drawRect(qMin(p1.x(),p2.x()),qMin(p1.y(),p2.y()),qAbs(p1.x()-p2.x()),qAbs(p1.y()-p2.y()));
                    }


                }
            }

            painter.end();
            painter.begin(this);
            painter.drawPixmap(0,0,temp_temp_pixmap);
            painter.end();


        }
        else
        {
            painter.begin(this);
            painter.drawPixmap(0,0,pixmap);
            painter.end();
        }
    }
    else if(current_global_state==PARA)
    {
        painter.begin(&pixmap);
        switch(current_shape)
        {
        case LINE:
            drawLine(painter);
            break;
        case CIRCLE:
            drawCircle(painter);
            break;
        case ELLIPSE:
            drawEllipse(painter);
            break;
        default:
            break;
        }
        painter.end();
        painter.begin(this);
        painter.drawPixmap(0,0,pixmap);
        current_global_state = NONE;
    }
    else if(current_global_state == FILL)
    {
        if(current_fill_state == FILL_READY)
        {
            painter.begin(&pixmap);
            current_fill_state = FILL_WAITING;
            fill(painter);
            painter.end();
        }
        painter.begin(this);
        painter.drawPixmap(0,0,pixmap);
        painter.end();
    }
    else
    {
        painter.begin(this);
        painter.drawPixmap(0,0,pixmap);
        painter.end();
    }
    painter.begin(this);
    for(int i =0;i<lines.size();i++){
        QPen pen = painter.pen();
        pen.setColor(linesColor[i]);
        pen.setWidth(linesWidth[i]);
        painter.setPen(pen);
        if(lines[i].size()>2)
            drawLine(painter,lines[i][2],lines[i][3]);
        else
            drawLine(painter,lines[i][0],lines[i][1]);
    }
    painter.end();

}
void draw::drawLine(QPainter &painter)
{
    drawLine(painter,p1,p2);
}

void draw::drawLine(QPainter &painter,QPoint p11,QPoint p22)
{
    if(p11.x()==p22.x()||qAbs((p11.y()-p22.y())/(p11.x()-p22.x()))>=1)
    {
        QPoint p3,p4;
        if(p11.y()<p22.y())
        {
            p3 = p11;
            p4 = p22;
        }
        else
        {
            p3 = p22;
            p4 = p11;
        }

        bool increase = (p4.x()>p3.x());

        const int a = p4.y()-p3.y();
        const int b = p3.x()-p4.x();
        int d = 0;
        int d_delta_when_positive = 0,d_delta_when_negative = 0;
        int x_delta_when_positive = 0,x_delta_when_negative = 0;
        if(increase)
        {

            d = a+2*b;
            d_delta_when_positive = 2*b;
            d_delta_when_negative = 2*a+2*b;
            x_delta_when_positive = 0;
            x_delta_when_negative = 1;
        }
        else
        {
            d = -a+2*b;
            d_delta_when_positive = 2*b-2*a;
            d_delta_when_negative = 2*b;
            x_delta_when_positive = -1;
            x_delta_when_negative = 0;
        }
        int i = p3.x();
        for(int j = p3.y();j<=p4.y();j++)
        {
            painter.drawPoint(i,j);


            if(d<0)
            {
                d += d_delta_when_negative;
                i += x_delta_when_negative;
            }
            else
            {
                d += d_delta_when_positive;
                i += x_delta_when_positive;
            }
        }

    }
    else
    {
        QPoint p3,p4;
        if(p11.x()<p22.x())
        {
            p3 = p11;
            p4 = p22;
        }
        else
        {
            p3 = p22;
            p4 = p11;
        }

        bool increase = (p4.y()>p3.y());

        const int a = p3.y()-p4.y();    //方程参数
        const int b = p4.x()-p3.x();    //方程参数
        int d = 0;
        int d_delta_when_positive = 0,d_delta_when_negative = 0;
        int y_delta_when_positive = 0,y_delta_when_negative = 0;
        if(increase)
        {
            //斜率大于0,小于1
            d = 2 * a+b;
            d_delta_when_positive = 2*a;
            d_delta_when_negative = 2*a+2*b;
            y_delta_when_positive = 0;
            y_delta_when_negative = 1;
        }
        else
        {
            d = 2 * a-b;
            d_delta_when_positive = 2*a-2*b;
            d_delta_when_negative = 2*a;
            y_delta_when_positive = -1;
            y_delta_when_negative = 0;
        }
        int j = p3.y();
        for(int i = p3.x();i<=p4.x();i++)
        {
            painter.drawPoint(i,j);


            if(d<0)
            {
                d += d_delta_when_negative;
                j += y_delta_when_negative;
            }
            else
            {
                d += d_delta_when_positive;
                j += y_delta_when_positive;
            }
        }

    }
}

void draw::drawCircle(QPainter &painter)
{
    int radius = qFloor(qSqrt(qPow(p1.x()-p2.x(),2)+qPow(p1.y()-p2.y(),2)))/2;
    int x_move = (p1.x()+p2.x())/2;    //圆心与原点在x轴的偏移
    int y_move = (p1.y()+p2.y())/2;    //圆心与原点在y轴的偏移

    int x = 0,y= radius;    //第一个点
    int d = 1-radius;       //变量d初始值
    int x_delta = 3,y_delta = 5-radius -radius; //x和y的delta值

    while(x<=y)
    {
        painter.drawPoint(x+x_move,y+y_move);
        painter.drawPoint(y+x_move,x+y_move);
        painter.drawPoint(-x+x_move,y+y_move);
        painter.drawPoint(y+x_move,-x+y_move);
        painter.drawPoint(x+x_move,-y+y_move);
        painter.drawPoint(-y+x_move,x+y_move);
        painter.drawPoint(-x+x_move,-y+y_move);
        painter.drawPoint(-y+x_move,-x+y_move);
        if(d<0)
        {
            d+=x_delta;
            x_delta+=2;
            y_delta+=2;
            x++;
        }
        else
        {
            d+=y_delta;
            x_delta+=2;
            y_delta+=4;
            x++;
            y--;
        }
    }
}

void draw::drawPolygon(QPainter &painter)
{
    if(points.size()<2)
        return;

    for(QVector<QPoint>::iterator i = points.begin();i!=points.end()-1;i++)
    {
        drawLine(painter,*i,*(i+1));
    }
}

void draw::drawCurve(QPainter &painter)
{
    if(points.size()<2)
        return;

    QVector<QPoint> ps = points;
    QVector<QPoint> result;
    for(double delta = 0;delta<1;delta+=0.01)
    {
        for(int size = points.size()-1;size>0;size--)
        {
            for(int i=0;i<size;i++)
            {
                ps[i] += delta*(ps[i+1]-ps[i]);
            }
        }
        result.push_back(ps.first());
    }
    for(int i=0;i<result.size()-1;i++)
    {
        painter.drawLine(result[i],result[i+1]);
    }
}

void draw::mousePressEvent(QMouseEvent *mpe)
{
    if(current_global_state==FILL)
    {
        p1=mpe->pos();
        current_fill_state = FILL_READY;
    }
    else if(current_global_state==DRAG)
    {
        if(current_shape == POLYGON || current_shape == CURVE)
        {
            if(current_dragging_state == WAITING)
            {
                setCursor(Qt::CrossCursor);
                current_dragging_state = DRAGGING;
            }
            else if(current_dragging_state == DECIDING)
            {
                editing_point_index = isEdit(mpe->pos());
                if(editing_point_index>=0)
                {
                    setCursor(Qt::SizeFDiagCursor);
                    current_dragging_state = EDITING;
                }
                else if(editing_point_index == -2)
                {
                    setCursor(Qt::SizeAllCursor);
                    current_dragging_state = MOVING;
                    setCursor(Qt::SizeAllCursor);
                    points_history = points;
                    center_history = center;
                }
                else if(editing_point_index == -1)
                {
                    save_current_shape();
                    setCursor(Qt::CrossCursor);
                    current_dragging_state = DRAGGING;
                }
            }
        }

        else
        {
            if(current_dragging_state == WAITING)
            {
                this->p1 = mpe->pos();
                this->p2 = mpe->pos();
                p1_backup = p1;
                p2_backup = p2;
                setRotate(180);
                setZoom(100);
                setCursor(Qt::CrossCursor);
                current_dragging_state = DRAGGING;
            }
            else if(current_dragging_state == DECIDING)
            {
                editing_point_index = isEdit(mpe->pos());
                if(editing_point_index>=0)
                {
                    setCursor(Qt::SizeFDiagCursor);
                    current_dragging_state = EDITING;
                }
                else if(editing_point_index == -2)
                {
                    setCursor(Qt::SizeAllCursor);
                    current_dragging_state = MOVING;
                    p1_history = p1;
                    p2_history = p2;
                }
                else if(editing_point_index == -1)
                {
                    save_current_shape();

                    if(current_shape == CLIP)
                    {
                        current_global_state = NONE;
                    }
                    else
                    {
                        this->p1 = mpe->pos();
                        this->p2 = mpe->pos();
                        p1_backup = p1;
                        p2_backup = p2;
                        setRotate(180);
                        setZoom(100);
                        setCursor(Qt::CrossCursor);
                        current_dragging_state = DRAGGING;
                    }

                }
            }
        }
    }

    update();
}
void draw::mouseMoveEvent(QMouseEvent *mpe)
{
    if(current_global_state!=DRAG)
        return;

    if(current_shape == POLYGON || current_shape == CURVE)
    {
        if(current_dragging_state==EDITING)
        {
            center = (center*points.size()-points[editing_point_index]+mpe->pos())/points.size();
            points[editing_point_index] = mpe->pos();
            points_backup[editing_point_index] = mpe->pos();

            if(current_shape == POLYGON && editing_point_index == 0)
            {
                points[points.size()-1]=mpe->pos();
                points_backup[points.size()-1] = mpe->pos();
            }
//            setRotate(180);
//            setZoom(100);
        }
        else if(current_dragging_state == MOVING)
        {
            center = center_history + mpe->pos()-center_history;
            for(int i=0;i<points.size();i++)
            {
                points[i] = points_history[i]+mpe->pos()-center_history;
                points_backup[i]=points_history[i]+mpe->pos()-center_history;
                setRotate(180);
                setZoom(100);
            }
        }
    }
    else
    {
        if(current_shape == CLIP)
        {
            clip();
        }
        if(current_dragging_state==DRAGGING)
        {
            this->p2 = mpe->pos();
            p2_backup = p2;
        }
        else if(current_dragging_state==EDITING)
        {
            if(editing_point_index == 1)
            {
                this->p1 = mpe->pos();
                p1_backup = p1;
//                setRotate(180);
//                setZoom(100);
            }
            else
            {
                this->p2 = mpe->pos();
                p2_backup = p2;
            }
        }else if(current_dragging_state == MOVING)
        {
            p1 =p1_history + (mpe->pos()-(p1_history+p2_history)/2);
            p2 =p2_history + (mpe->pos()-(p1_history+p2_history)/2);
            p1_backup = p1;
            p2_backup = p2;
//            setRotate(180);
//            setZoom(100);
        }
    }

    update();
}
void draw::mouseReleaseEvent(QMouseEvent *mpe)
{
    Q_UNUSED(mpe);

    if(current_global_state!=DRAG)
        return;

    //Polygon or Curve
    if(current_shape == POLYGON || current_shape == CURVE)
    {
        if(current_dragging_state == DRAGGING)
        {
            //Clicked on any existing point
            if(points.size()>2&&(qPow(mpe->pos().x()-points[0].x(),2)+qPow(mpe->pos().y()-points[0].y(),2)<100))
            {
                if(current_shape == POLYGON)
                {
                    points.push_back(points[0]);
                    points_backup.push_back(points[0]);
                }

                //Get center point
                center.setX(0);
                center.setY(0);
                for(int i=0;i<points.size();i++)
                {
                    center += points[i];
                }
                center/=points.size();

                setCursor(Qt::ArrowCursor);
                current_dragging_state = DECIDING;
                setRotate(180);
                setZoom(100);
            }
            else if(current_shape == CURVE && points.size()>2 &&(qPow(mpe->pos().x()-points[points.size()-1].x(),2)+qPow(mpe->pos().y()-points[points.size()-1].y(),2)<100))
            {
                //Get center point
                center.setX(0);
                center.setY(0);
                for(int i=0;i<points.size();i++)
                {
                    center += points[i];
                }
                center/=points.size();

                setCursor(Qt::ArrowCursor);
                current_dragging_state = DECIDING;
                setRotate(180);
                setZoom(100);
            }
            else
            {
                points.push_back(mpe->pos());
                points_backup.push_back(mpe->pos());
            }
        }
        else if (current_dragging_state == EDITING)
        {
            setCursor(Qt::ArrowCursor);
            current_dragging_state = DECIDING;
//            setRotate(180);
//            setZoom(100);
        }
        else if(current_dragging_state==MOVING)
        {
            setCursor(Qt::ArrowCursor);
            current_dragging_state = DECIDING;
//            setRotate(180);
//            setZoom(100);
        }
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        current_dragging_state = DECIDING;
//        setRotate(180);
//        setZoom(100);

        //Debug
        if(current_shape == CLIP){
            clip();
        }
    }

    update();
}

void draw::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(current_shape == CURVE && current_dragging_state == DRAGGING)
    {
        //初始化
        center.setX(0);
        center.setY(0);
        //各点之和
        for(int i=0;i<points.size();i++)
        {
            center += points[i];
        }
        //求均值
        center/=points.size();
        setCursor(Qt::ArrowCursor);
        current_dragging_state = DECIDING;
        setRotate(180);
        setZoom(100);
    }
}

void draw::drawEllipse(QPainter &painter)
{
    int rx = ellipse_rx,ry=ellipse_ry;
    int xc = ellipse_xc,yc =ellipse_yc;

    int p = ry*ry-rx*rx*ry+rx*rx/4; //区域1初始决策参数
    int x=0,y=ry;                   //初始点
    while(ry*ry*x<rx*rx*y)
    {
        if(rotate == 180){
            painter.drawPoint(x+xc,y+yc);
            painter.drawPoint(-x+xc,y+yc);
            painter.drawPoint(x+xc,-y+yc);
            painter.drawPoint(-x+xc,-y+yc);
        }else{
            double radian = qDegreesToRadians(double(rotate-180));
            double cos = qCos(radian);
            double sin = qSin(radian);

            painter.drawPoint(int(xc+x*cos-y*sin),int(yc+x*sin+y*cos));
            painter.drawPoint(int(xc-x*cos-y*sin),int(yc-x*sin+y*cos));
            painter.drawPoint(int(xc+x*cos+y*sin),int(yc+x*sin-y*cos));
            painter.drawPoint(int(xc-x*cos+y*sin),int(yc-x*sin-y*cos));
        }

        if(p<=0)
        {
            x++;
            p = p+ 2*ry*ry*x+ry*ry;
        }
        else
        {
            x++;
            y--;
            p = p + 2*ry*ry*x-2*rx*rx*y+ry*ry;
        }
    }

    p = ry*ry*x*x+rx*rx*(y-1)*(y-1)-rx*rx*ry*ry;//区域2初始决策参
    while(y>=0)
    {
        if(rotate== 180){
            painter.drawPoint(x+xc,y+yc);
            painter.drawPoint(-x+xc,y+yc);
            painter.drawPoint(x+xc,-y+yc);
            painter.drawPoint(-x+xc,-y+yc);
        }
        else{
            double radian = qDegreesToRadians(double(rotate-180));
            double cos = qCos(radian);
            double sin = qSin(radian);

            painter.drawPoint(int(xc+x*cos-y*sin),int(yc+x*sin+y*cos));
            painter.drawPoint(int(xc-x*cos-y*sin),int(yc-x*sin+y*cos));
            painter.drawPoint(int(xc+x*cos+y*sin),int(yc+x*sin-y*cos));
            painter.drawPoint(int(xc-x*cos+y*sin),int(yc-x*sin-y*cos));
        }
        if(p>0)
        {
            y--;
            p = p-2*rx*rx*y+rx*rx;
        }
        else
        {
            x++;
            y--;
            p = p+ 2*ry*ry*x-2*rx*rx*y+rx*rx;
        }
    }
}


void draw::toDrawLineByPara(int a,int b,int c)
{
    save_current_shape();
    current_dragging_state = INVALID;
    current_global_state = PARA;
    current_shape = LINE;

    int max_x = this->size().width();

    p1.setX(0);
    p1.setY(-c/b);
    p2.setX(max_x);
    p2.setY((-c-a*max_x)/b);
    p1_backup = p1;
    p2_backup = p2;
    setRotate(180);
    setZoom(100);
    update();
}


void draw::toDrawCircleByPara(int x0,int y0,int r)
{
    save_current_shape();
    current_dragging_state = INVALID;
    current_global_state = PARA;
    current_shape = CIRCLE;

    p1.setX(x0-r);
    p1.setY(y0);
    p2.setX(x0+r);
    p2.setY(y0);
    p1_backup = p1;
    p2_backup = p2;
    setRotate(180);
    setZoom(100);

    update();
}

void draw::toDrawEllipseByPara(int xc,int rx,int yc,int ry)
{
    save_current_shape();
    current_dragging_state = INVALID;
    current_global_state = PARA;
    current_shape = ELLIPSE;

    ellipse_xc = xc;
    ellipse_yc = yc;
    ellipse_rx = rx;
    ellipse_ry = ry;

    update();
}

void draw::clearPixmap()
{
    temp_pixmap.fill(Qt::white);
    pixmap = temp_pixmap;
    points.clear();
    points_backup.clear();
    if(current_dragging_state == DECIDING)
    {
        current_dragging_state = WAITING;
    }

    lines.clear();
    linesColor.clear();
    linesWidth.clear();
    circles.clear();
    ellipses.clear();
    polygons.clear();
    curves.clear();
    update();

}

void draw::savePixmap(QString path)
{
    save_current_shape();
    if(current_global_state == DRAG)
    {
        current_dragging_state = WAITING;
    }
    pixmap.save(path,"png");
}


void draw::toDrawShapeByDrag(shape shape_to_draw)
{
    save_current_shape();
    current_global_state = DRAG;
    current_dragging_state = WAITING;
    current_shape = shape_to_draw;
    if(shape_to_draw==POLYGON && first_time_using_polygon)
    {
        first_time_using_polygon = false;

        polygon_info x;
        x.setModal(true);
        x.show();
        x.exec();
    }else if(shape_to_draw==CURVE && first_time_using_curve){
        first_time_using_curve = false;

        curve_info x;
        x.setModal(true);
        x.show();
        x.exec();
    }
    update();
}

int draw::isEdit(QPoint p)
{

    if(current_shape == POLYGON || current_shape == CURVE)
    {
        if(qPow(center.x()-p.x(),2)+qPow(center.y()-p.y(),2)<100)
        {
            return -2;
        }
        for(int i=0;i<points.size();i++)
        {
            if(qPow(points[i].x()-p.x(),2)+qPow(points[i].y()-p.y(),2)<100)
            {
                return i;
            }
        }
    }
    else
    {
        QPoint c = (p1+p2)/2;
        if(qPow(c.x()-p.x(),2)+qPow(c.y()-p.y(),2)<100)
        {
            return -2;
        }
        int p1_dis = qFloor(qPow(p1.x()-p.x(),2)+qPow(p1.y()-p.y(),2));
        int p2_dis = qFloor(qPow(p2.x()-p.x(),2)+qPow(p2.y()-p.y(),2));
        if(p1_dis<p2_dis)
        {
            if(p1_dis<100)
            {
                return 1;
            }
        }
        else
        {
            if(p2_dis<100)
            {
                return 2;
            }
        }
    }

    return -1;
}

void draw::draw_small_point(QPainter &painter,QPoint p)
{
    painter.setBrush(Qt::blue);
    QRectF rectangle(p.x()-4,p.y()-4,8,8);
    //此处是为了画出让用户编辑的小圆点，非二维绘图算法实现，因此直接调用Qt框架API
    painter.drawEllipse(rectangle);
    painter.setBrush(Qt::NoBrush);
}
void draw::save_current_shape()
{
    if(current_shape == LINE)
    {
        QVector<QPoint> oneLine;
        oneLine.push_back(p1);
        oneLine.push_back(p2);
        lines.push_back(oneLine);
        linesColor.push_back(currentColor);
        linesWidth.push_back(currentPenWidith);
    }else if(current_shape == CLIP){
        for(int i=0;i<lines.size();i++){
            if(lines[i].size()>2){
                lines[i][0] = lines[i][2];
                lines[i][1] = lines[i][3];
                lines[i].pop_back();
                lines[i].pop_back();
            }
        }
    }else{
        pixmap = temp_pixmap;
    }
    points.clear();
    points_backup.clear();

    rotate = 180;
    zoom = 100;
}

void draw::toFill()
{
    save_current_shape();
    current_global_state = FILL;
    current_fill_state = FILL_WAITING;

    update();
}

void draw::fill(QPainter &painter)
{
    QImage image = pixmap.toImage();
    if(image.pixelColor(p1)!=Qt::white)
    {
        return;
    }

    QPoint temp,temp_temp;
    QPoint delta[]={QPoint(1,0),QPoint(-1,0),QPoint(0,1),QPoint(0,-1)};

    QVector<QPoint> pv;     //保存未处理点的栈
    pv.push_back(p1);       //种子点进栈

    QPen pen = painter.pen();
    pen.setColor(currentColor);
    painter.setPen(pen);

    while(!pv.empty())
    {
        //取出一个点
        temp = pv.takeLast();
        //涂色
        image.setPixelColor(temp,Qt::black);
        painter.drawPoint(temp);
        //获取相邻点并判断
        for(int i=0;i<4;i++)
        {
            temp_temp = temp+delta[i];
            if(image.pixelColor(temp_temp)==Qt::white&&temp_temp.x()>=0&&temp_temp.x()<pixmap.width()&&temp_temp.y()>=0&&temp_temp.y()<pixmap.height())
            {
                //符合条件，进栈
                pv.push_back(temp_temp);
            }
        }
    }
    update();

}

void draw::draw_small_rectangle(QPainter &painter,QPoint p)
{
    painter.setBrush(Qt::blue);

    //此处是为了画出让用户拖动的按钮，非二维绘图算法实现，因此直接调用Qt框架API
    painter.drawRect(p.x()-7,p.y()-7,14,14);

    painter.setBrush(Qt::NoBrush);
}

void draw::clip()
{
    qDebug()<<"clip";

    int x_min = (p1.x()<p2.x()?p1.x():p2.x());
    int x_max = (p1.x()>p2.x()?p1.x():p2.x());
    int y_min = (p1.y()<p2.y()?p1.y():p2.y());
    int y_max = (p1.y()>p2.y()?p1.y():p2.y());
    for(int index=0;index<lines.size();index++){
        QVector<QPoint> line = lines[index];

        int delta_x = line[1].x()-line[0].x();
        int delta_y = line[1].y()- line[0].y();

        int p[4] = {-delta_x,delta_x,-delta_y,delta_y};
        int q[4] = {line[0].x()-x_min,x_max - line[0].x(),line[0].y()-y_min,y_max - line[0].y()};

        double u1 = 0,u2 = 1;
        bool done = false;
        for(int i=0;i<4;i++){
            if(p[i]==0 ){
                if(q[i]<0){
//                    lines.remove(index);
                    if(lines[index].size()>2)
                    {
                        lines[index][2] = QPoint(0,0);
                        lines[index][3] = QPoint(0,0);
                    }
                    else
                    {
                        lines[index].push_back(QPoint(0,0));
                        lines[index].push_back(QPoint(0,0));
                    }

                    done = true;
                    break;
                }
                //else do nothing
            }else{
                if(p[i]<0){
                    u1 = qMax(u1,double(q[i])/double(p[i]));
                }else if(p[i]>0){
                    u2 = qMin(u2,double(q[i])/double(p[i]));
                }
            }
        }
        if(done){
            continue;
        }else{
            if(u1>u2){
                if(lines[index].size()>2)
                {
                    lines[index][2] = QPoint(0,0);
                    lines[index][3] = QPoint(0,0);
                }
                else
                {
                    lines[index].push_back(QPoint(0,0));
                    lines[index].push_back(QPoint(0,0));
                }
            }else{
                QPoint p_before = lines[index][0];
                QPoint p_delta_before = lines[index][1]-lines[index][0];
                if(lines[index].size()>2)
                {
                    lines[index][2] = p_before + u1*(p_delta_before);
                    lines[index][3] = p_before + u2*(p_delta_before);
                }
                else
                {
                    lines[index].push_back(p_before+u1*(p_delta_before));
                    lines[index].push_back(p_before + u2*(p_delta_before));
                }
            }
        }


    }
    update();
}

void draw::rotateChanged(int value)
{
    if(zoom != 100){
        p1_backup = p1;
        p2_backup = p2;
        points_backup = points;

        setZoom(100);
    }
    if(current_dragging_state == DECIDING){
        rotate = value;
        if(current_shape == ELLIPSE){
            //do nothing
        }else if(current_shape == LINE||current_shape == CIRCLE){
            double radian = qDegreesToRadians(double(rotate-180));
            double cos = qCos(radian);
            double sin = qSin(radian);

            QPoint center = (p1_backup+p2_backup)/2;
            p1.setX(int(center.x()+(p1_backup-center).x()*cos-(p1_backup-center).y()*sin));
            p1.setY(int(center.y()+(p1_backup-center).x()*sin+(p1_backup-center).y()*cos));
            p2.setX(int(center.x()+(p2_backup-center).x()*cos-(p2_backup-center).y()*sin));
            p2.setY(int(center.y()+(p2_backup-center).x()*sin+(p2_backup-center).y()*cos));


        }else if(current_shape == POLYGON || current_shape == CURVE){
            double radian = qDegreesToRadians(double(rotate-180));
            double cos = qCos(radian);
            double sin = qSin(radian);

            QPoint center = QPoint(0,0);
            for(int i=0;i<points_backup.size();i++){
                center += points_backup[i];

            }
            center /= points_backup.size();
            for(int i=0;i<points.size();i++){
                points[i].setX(int(center.x()+(points_backup[i]-center).x()*cos-(points_backup[i]-center).y()*sin));
                points[i].setY(int(center.y()+(points_backup[i]-center).x()*sin+(points_backup[i]-center).y()*cos));
            }
        }
        update();
    }
}

void draw::zoomChanged(int value)
{
    zoom = value;
    if(current_shape != ELLIPSE && rotate != 180){
        p1_backup = p1;
        p2_backup = p2;
        points_backup = points;
        setRotate(180);
    }
    if(current_dragging_state == DECIDING)
    {
        double s = double(value)/100.0;
        if(current_shape == LINE || current_shape == CIRCLE ||current_shape == ELLIPSE)
        {

            QPoint center = (p1_backup+p2_backup)/2;

            p1 = p1_backup*s+center*(1-s);
            p2 = p2_backup*s+center*(1-s);
            update();

        }else if(current_shape == POLYGON || current_shape == CURVE){
            qDebug()<<s;
            QPoint center = QPoint(0,0);
            for(int i=0;i<points_backup.size();i++){
                center += points_backup[i];

            }
            center /= points_backup.size();
            for(int i=0;i<points.size();i++){
                points[i] = points_backup[i]*s+center*(1-s);
            }
            update();
        }
    }
}

void draw::setZoom(int value)
{
    zoom = value;
    emit setZoomSignal(value);
}
void draw::setRotate(int value)
{
    rotate = value;
    emit setZoomSignal(value);
}

void draw::changeCurrentColor(QColor color)
{
    currentColor = color;
}

void draw::changePenWidth(int width)
{
   currentPenWidith = width;
}
