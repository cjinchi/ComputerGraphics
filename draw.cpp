#include "draw.h"
#include <QVector>
#include <QRectF>
#include <QMessageBox>
#include "polygon_info.h"
#include <QImage>
#include <QCursor>

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

    ellipse_xc = 0;
    ellipse_yc = 0;
    ellipse_rx = 0;
    ellipse_ry = 0;

    first_time_using_polygon = true;

    setCursor(Qt::ArrowCursor);

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
            }
            painter.end();

            temp_temp_pixmap = temp_pixmap;
            painter.begin(&temp_temp_pixmap);

            painter.setPen(Qt::blue);
            if(current_shape == POLYGON)
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
                if(current_shape==ELLIPSE)
                {
                    painter.setPen(Qt::DotLine);
                    painter.drawRect(qMin(p1.x(),p2.x()),qMin(p1.y(),p2.y()),qAbs(p1.x()-p2.x()),qAbs(p1.y()-p2.y()));

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

}

void draw::drawLine(QPainter &painter)
{
    if(p1.x()==p2.x()||qAbs((p1.y()-p2.y())/(p1.x()-p2.x()))>=1)
    {
        QPoint p3,p4;
        if(p1.y()<p2.y())
        {
            p3 = p1;
            p4 = p2;
        }
        else
        {
            p3 = p2;
            p4 = p1;
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
        if(p1.x()<p2.x())
        {
            p3 = p1;
            p4 = p2;
        }
        else
        {
            p3 = p2;
            p4 = p1;
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
        p1 = *i;
        p2 = *(i+1);
        drawLine(painter);
    }
}

void draw::mousePressEvent(QMouseEvent *mpe)
{
    if(current_global_state==FILL)
    {
        p1=mpe->pos();
        current_fill_state = FILL_READY;
    }else if(current_global_state==DRAG)
    {
        if(current_shape == POLYGON)
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

                    this->p1 = mpe->pos();
                    this->p2 = mpe->pos();
                    setCursor(Qt::CrossCursor);
                    current_dragging_state = DRAGGING;
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

    if(current_shape == POLYGON)
    {
        if(current_dragging_state==EDITING)
        {
            center = (center*points.size()-points[editing_point_index]+mpe->pos())/points.size();
            points[editing_point_index] = mpe->pos();
            if(editing_point_index == 0)
            {
                points[points.size()-1]=mpe->pos();
            }
        }
        else if(current_dragging_state == MOVING)
        {
            center = center_history + mpe->pos()-center_history;
            for(int i=0;i<points.size();i++)
            {
                points[i] = points_history[i]+mpe->pos()-center_history;
            }
        }
    }
    else
    {
        if(current_dragging_state==DRAGGING)
        {
            this->p2 = mpe->pos();
        }
        else if(current_dragging_state==EDITING)
        {
            if(editing_point_index == 1)
            {
                this->p1 = mpe->pos();
            }
            else
            {
                this->p2 = mpe->pos();
            }
        }else if(current_dragging_state == MOVING)
        {
            p1 =p1_history + (mpe->pos()-(p1_history+p2_history)/2);
            p2 =p2_history + (mpe->pos()-(p1_history+p2_history)/2);
        }
    }

    update();
}
void draw::mouseReleaseEvent(QMouseEvent *mpe)
{
    Q_UNUSED(mpe);

    qDebug()<<current_global_state;
    if(current_global_state!=DRAG)
        return;

    if(current_shape == POLYGON)
    {
        qDebug()<<"poly";
        if(current_dragging_state == DRAGGING)
        {
            if(points.size()>2&&(qPow(mpe->pos().x()-points[0].x(),2)+qPow(mpe->pos().y()-points[0].y(),2)<100))
            {
                points.push_back(points[0]);
                center.setX(0);
                center.setY(0);
                for(int i=0;i<points.size();i++)
                {
                    center += points[i];
                }
                center/=points.size();
                setCursor(Qt::ArrowCursor);
                current_dragging_state = DECIDING;
            }
            else
            {
                points.push_back(mpe->pos());
            }
        }
        else if (current_dragging_state == EDITING)
        {
            setCursor(Qt::ArrowCursor);
            current_dragging_state = DECIDING;
        }
        else if(current_dragging_state==MOVING)
        {
            setCursor(Qt::ArrowCursor);
            current_dragging_state = DECIDING;
        }
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        current_dragging_state = DECIDING;
    }

    update();
}

void draw::drawEllipse(QPainter &painter)
{
    int rx = ellipse_rx,ry=ellipse_ry;
    int xc = ellipse_xc,yc =ellipse_yc;

    int p = ry*ry-rx*rx*ry+rx*rx/4; //区域1初始决策参数
    int x=0,y=ry;                   //初始点
    while(ry*ry*x<rx*rx*y)
    {
        painter.drawPoint(x+xc,y+yc);
        painter.drawPoint(-x+xc,y+yc);
        painter.drawPoint(x+xc,-y+yc);
        painter.drawPoint(-x+xc,-y+yc);
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
        painter.drawPoint(x+xc,y+yc);
        painter.drawPoint(-x+xc,y+yc);
        painter.drawPoint(x+xc,-y+yc);
        painter.drawPoint(-x+xc,-y+yc);
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
    if(current_dragging_state == DECIDING)
    {
        current_dragging_state = WAITING;
    }
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
    }
    update();
}

int draw::isEdit(QPoint p)
{

    if(current_shape == POLYGON)
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
    painter.drawEllipse(rectangle);
    painter.setBrush(Qt::NoBrush);
}
void draw::save_current_shape()
{
    pixmap = temp_pixmap;
    points.clear();
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
    qDebug()<<"iinfi";
    QImage image = pixmap.toImage();
    if(image.pixelColor(p1)!=Qt::white)
    {
        qDebug()<<"no white";
        return;
    }

    QVector<QPoint> pv;
    QPoint temp,temp_temp;
    QPoint delta[]={QPoint(1,0),QPoint(-1,0),QPoint(0,1),QPoint(0,-1)};
    pv.push_back(p1);

    qDebug()<<"width"<<image.width();
    qDebug()<<"height"<<image.height();
    while(!pv.empty())
    {
        temp = pv.takeLast();
        image.setPixelColor(temp,Qt::black);
        painter.drawPoint(temp);
        for(int i=0;i<4;i++)
        {
            temp_temp = temp+delta[i];
            if(image.pixelColor(temp_temp)==Qt::white&&temp_temp.x()>=0&&temp_temp.x()<pixmap.width()&&temp_temp.y()>=0&&temp_temp.y()<pixmap.height())
//            if(image.pixelColor(temp_temp)==Qt::white)
            {
                pv.push_back(temp_temp);
            }
        }
    }
    update();

}

void draw::draw_small_rectangle(QPainter &painter,QPoint p)
{
    painter.setBrush(Qt::blue);
    painter.drawRect(p.x()-7,p.y()-7,14,14);
    painter.setBrush(Qt::NoBrush);
}
