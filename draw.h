#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QVector>
#include "stable.h"
#include "config.h"
#include "state.h"
#include "colored_point.h"
#include "polygon_info.h"

class draw : public QWidget
{
    Q_OBJECT
public:
    explicit draw(QWidget *parent = nullptr);

private:
    shape current_shape;
    global_state current_global_state;
    dragging_state current_dragging_state;
    fill_state current_fill_state;

    QPixmap pixmap;
    QPixmap temp_pixmap;
    QPixmap temp_temp_pixmap;
    QPoint p1,p2;
    QVector<QPoint> points;

    int ellipse_xc,ellipse_yc;
    int ellipse_rx,ellipse_ry;
    int editing_point_index;

    bool first_time_using_polygon;
private:

    void drawLine(QPainter &painter);
    void drawCircle(QPainter &painter);
    void drawEllipse(QPainter &painter);
    void drawPolygon(QPainter &painter);
    void fill(QPainter &painter);

    void mousePressEvent(QMouseEvent *mpe);
    void mouseReleaseEvent(QMouseEvent *mpe);
    void mouseMoveEvent(QMouseEvent *mpe);
    void paintEvent(QPaintEvent *e);

    int isEdit(QPoint p);
    void draw_small_point(QPainter &painter,QPoint p);
    void save_current_shape();


signals:

public slots:
    void toDrawShapeByDrag(shape shape_to_draw);

    void toDrawLineByPara(int a,int b,int c);

    void toDrawCircleByPara(int x0,int y0,int r);

    void toDrawEllipseByPara(int xc,int rx,int yc,int ry);

    void clearPixmap();

    void toFill();

    void savePixmap(QString path);
};

#endif // DRAW_H
