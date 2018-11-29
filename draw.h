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
    bool able_dragging;
    dragging_state current_dragging_state;
    bool should_reload_pixmap;

    QPixmap pixmap;
    QPixmap temp_pixmap;
    QPoint p1,p2;

    int ellipse_xc,ellipse_yc;
    int ellipse_rx,ellipse_ry;
private:

    void drawLine(QPainter &painter);
    void drawCircle(QPainter &painter);
    void drawEllipse(QPainter &painter);

    void mousePressEvent(QMouseEvent *mpe);
    void mouseReleaseEvent(QMouseEvent *mpe);
    void mouseMoveEvent(QMouseEvent *mpe);
    void paintEvent(QPaintEvent *e);


signals:

public slots:
    void toDrawShapeByDrag(shape shape_to_draw);

    void toDrawLineByPara(int a,int b,int c);

    void toDrawCircleByPara(int x0,int y0,int r);

    void toDrawEllipseByPara(int xc,int rx,int yc,int ry);

    void clearPixmap();

    void savePixmap(QString path);
};

#endif // DRAW_H
