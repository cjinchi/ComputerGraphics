#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include "stable.h"
#include "config.h"
#include "state.h"
#include "colored_point.h"

class draw : public QWidget
{
    Q_OBJECT
public:
    explicit draw(QWidget *parent = nullptr);

private:
    shape current_shape;
    input_type current_input_type;
    bool able_to_drag;

    QPixmap pixmap;
    QPoint p1,p2;
    QVector<colored_point> buffer;
    int moveTime;

    int ellipse_xc,ellipse_yc;
    int ellipse_rx,ellipse_ry;
private:
    void paintEvent(QPaintEvent *e);
    void drawing(bool temporary);
    void drawLine(bool temporary,QPainter &painter);
    void drawCircle(bool temporary,QPainter &painter);
    void drawEllipse(bool temporary,QPainter &painter);
    colored_point getColoredPoint(int x,int y);
    void releaseBuffer(QPainter &painter);

    void mousePressEvent(QMouseEvent *mpe);
    void mouseReleaseEvent(QMouseEvent *mpe);
    void mouseMoveEvent(QMouseEvent *mpe);


signals:

public slots:
    void setCurrentShape(shape s);
};

#endif // DRAW_H
