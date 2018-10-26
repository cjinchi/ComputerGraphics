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

        const int a = p3.x()-p4.x();
        const int b = p4.y()-p3.y();
        int d = 2 * a+b;
        const int delta_when_positive = a;
        const int delta_when_negative = a+b;
        int j = p3.x();
        for(int i = p3.y();i<=p4.y();i++)
        {
            if(temporary)
            {
                buffer.push_back(getColoredPoint(j,i));
            }
            painter.drawPoint(j,i);
            if(d<0)
            {
                d+=delta_when_negative;
                j++;
            }
            else
            {
                d+=delta_when_positive;
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

        const int a = p3.y()-p4.y();
        const int b = p4.x()-p3.x();
        int d = 2 * a+b;
        const int delta_when_positive = a;
        const int delta_when_negative = a+b;
        const int j_delta = (p3.y()<p4.y()?1:-1);
        int j = p3.y();
        for(int i = p3.x();i<=p4.x();i++)
        {
            if(d<0)
            {
                d+=delta_when_negative;
                j+=j_delta;
            }
            else
            {
                d+=delta_when_positive;
            }
            if(temporary)
            {
                buffer.push_back(getColoredPoint(i,j));
            }
            painter.drawPoint(i,j);
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

