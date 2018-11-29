#include "draw.h"

draw::draw(QWidget *parent) : QWidget(parent)
{

    able_dragging = false;
    should_reload_pixmap = false;
    current_dragging_state = INVALID;

    pixmap = QPixmap(WINDOW_WIDTH,WINDOW_HEIGHT);
    pixmap.fill(Qt::white);
    temp_pixmap = pixmap;

    p1 = QPoint(0,0);
    p2 = QPoint(0,0);

    ellipse_xc = 0;
    ellipse_yc = 0;
    ellipse_rx = 0;
    ellipse_ry = 0;
}

void draw::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    temp_pixmap = pixmap;
    QPainter painter;


    if(able_dragging)
    {
        if(current_dragging_state == DRAGGING || current_dragging_state == ALMOST_DONE)
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
            }
            painter.end();

            if(current_dragging_state == ALMOST_DONE)
            {
                current_dragging_state = WAITING;
                pixmap = temp_pixmap;
            }
        }

    }

    painter.begin(this);
    painter.drawPixmap(0,0,temp_pixmap);
    painter.end();

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

void draw::mousePressEvent(QMouseEvent *mpe)
{
    if(!able_dragging)
        return;

    this->p1 = mpe->pos();
    this->p2 = mpe->pos();

    current_dragging_state = DRAGGING;

    update();


}
void draw::mouseMoveEvent(QMouseEvent *mpe)
{
    if(current_dragging_state != DRAGGING)
        return;

    this->p2 = mpe->pos();

    update();
}
void draw::mouseReleaseEvent(QMouseEvent *mpe)
{
    Q_UNUSED(mpe);

    if(current_dragging_state!=DRAGGING)
        return;

    current_dragging_state = ALMOST_DONE;

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

    p = qFloor(ry*ry*(x+0.5)*(x+0.5)+rx*rx*(y-1)*(y-1)-rx*rx*ry*ry);//区域2初始决策参数
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
    able_dragging = false;
    current_shape = LINE;

    int max_x = this->size().width();

    p1.setX(0);
    p1.setY(-c/b);
    p2.setX(max_x);
    p2.setY((-c-a*max_x)/b);

}


void draw::toDrawCircleByPara(int x0,int y0,int r)
{
    able_dragging = false;
    current_shape = CIRCLE;

    p1.setX(x0-r);
    p1.setY(y0);
    p2.setX(x0+r);
    p2.setY(y0);

    //    drawing(false);
}

void draw::toDrawEllipseByPara(int xc,int rx,int yc,int ry)
{
    able_dragging = false;
    current_shape = ELLIPSE;

    ellipse_xc = xc;
    ellipse_yc = yc;
    ellipse_rx = rx;
    ellipse_ry = ry;
}

void draw::clearPixmap()
{
    temp_pixmap.fill(Qt::white);
    pixmap = temp_pixmap;
    update();

}

void draw::savePixmap(QString path)
{
    pixmap.save(path,"png");
}


void draw::toDrawShapeByDrag(shape shape_to_draw)
{
    able_dragging = true;
    current_dragging_state = WAITING;
    current_shape = shape_to_draw;
}
