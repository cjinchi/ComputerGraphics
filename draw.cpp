#include "draw.h"

draw::draw(QWidget *parent) : QWidget(parent)
{
    this->setCurrentShape(LINE);

    pixmap = QPixmap(WINDOW_WIDTH,WINDOW_HEIGHT);
    pixmap.fill(Qt::white);

    p1 = QPoint(0,0);
    p2 = QPoint(0,0);

    buffer.reserve(10000);
}

void draw::setCurrentShape(shape s)
{
    this->current_shape = s;
}

void draw::drawing(bool temporary)
{
    QPainter painter(&pixmap);

    switch(this->current_shape)
    {
    case LINE:
        drawLine(temporary,painter);
        break;
    case CIRCLE:
        drawCircle(temporary,painter);
        break;
    }

    update();
}

void draw::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.drawPixmap(0,0,WINDOW_WIDTH,WINDOW_HEIGHT,pixmap);
}

void draw::drawLine(bool temporary,QPainter &painter)
{
    QPen init_pen = painter.pen();
    this->releaseBuffer(painter);
    painter.setPen(init_pen);

    if(p1.x()==p2.x()||qAbs((p1.y()-p2.y())/(p1.x()-p2.x()))>1)
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
            x_delta_when_positive = 1;
            x_delta_when_negative = 0;
        }
        else
        {
            d = -a+2*b;
            d_delta_when_positive = 2*b-2*a;
            d_delta_when_negative = 2*b;
            x_delta_when_positive = 0;
            x_delta_when_negative = -1;
        }
        int i = p3.x();
        for(int j = p3.y();j<=p4.y();j++)
        {
            if(temporary)
            {
                buffer.push_back(getColoredPoint(i,j));
            }
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

        const int a = p3.y()-p4.y();
        const int b = p4.x()-p3.x();
        int d = 0;
        int d_delta_when_positive = 0,d_delta_when_negative = 0;
        int y_delta_when_positive = 0,y_delta_when_negative = 0;
        if(increase)
        {
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


            if(temporary)
            {
                buffer.push_back(getColoredPoint(i,j));
            }
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

void draw::drawCircle(bool temporary,QPainter &painter)
{
    Q_UNUSED(temporary);
    Q_UNUSED(painter);
}

colored_point draw::getColoredPoint(int x,int y)
{
    QRgb rgb = pixmap.copy(x,y,1,1).toImage().pixel(0,0);
    return colored_point(x,y,rgb);
}

void draw::mousePressEvent(QMouseEvent *mpe)
{
    this->p1 = mpe->pos();
    this->p2 = mpe->pos();
}
void draw::mouseMoveEvent(QMouseEvent *mpe)
{
    this->p2 = mpe->pos();
    this->drawing(true);

}
void draw::mouseReleaseEvent(QMouseEvent *mpe)
{
    this->p2=mpe->pos();
    this->drawing(false);
}

void draw::releaseBuffer(QPainter &painter)
{
    if(!this->buffer.empty())
    {
        for(QVector<colored_point>::iterator i=buffer.begin();i!=buffer.end();i++)
        {
            QColor color(i->rgb);
            painter.setPen(color);
            painter.drawPoint(i->x,i->y);
        }
        buffer.clear();
    }
}

