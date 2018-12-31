#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QVector>
#include "stable.h"
#include "config.h"
#include "state.h"
#include "colored_point.h"
#include "polygon_info.h"
#include "curve_info.h"
#include <QColorDialog>

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
    QPoint p1_backup,p2_backup;
    QVector<QPoint> points;
    QVector<QPoint> points_backup;
    QPoint p1_history,p2_history;
    QVector<QPoint> points_history;
    QPoint center,center_history;
    QColor currentColor;
    QVector<QColor> linesColor;
    int currentPenWidith;
    QVector<int> linesWidth;

    int ellipse_xc,ellipse_yc;
    int ellipse_rx,ellipse_ry;
    int editing_point_index;

    bool first_time_using_polygon;
    bool first_time_using_curve;

    int rotate;
    int zoom;

    //Data
    QVector<QVector<QPoint>> lines;
    QVector<QVector<QPoint>> circles;
    QVector<QVector<QPoint>> ellipses;
    QVector<QVector<QPoint>> polygons;
    QVector<QVector<QPoint>> curves;
private:

    //Algorithm
    void drawLine(QPainter &painter);
    void drawLine(QPainter &painter,QPoint p11,QPoint p22);
    void drawCircle(QPainter &painter);
    void drawEllipse(QPainter &painter);
    void drawPolygon(QPainter &painter);
    void drawCurve(QPainter &painter);
    void fill(QPainter &painter);
    void clip();

    //System
    void mousePressEvent(QMouseEvent *mpe);
    void mouseReleaseEvent(QMouseEvent *mpe);
    void mouseMoveEvent(QMouseEvent *mpe);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);


    //Supporting
    int isEdit(QPoint p);
    void draw_small_point(QPainter &painter,QPoint p);
    void draw_small_rectangle(QPainter &painter,QPoint p);
    void save_current_shape();
    void setZoom(int value);
    void setRotate(int value);

signals:
    void setSlidersVisible(bool enable);
    void setZoomSignal(int value);
    void setRotateSignal(int value);

public slots:
    void toDrawShapeByDrag(shape shape_to_draw);

    void toDrawLineByPara(int a,int b,int c);

    void toDrawCircleByPara(int x0,int y0,int r);

    void toDrawEllipseByPara(int xc,int rx,int yc,int ry);

    void clearPixmap();

    void toFill();

    void savePixmap(QString path);

    void rotateChanged(int value);

    void zoomChanged(int value);

    void changeCurrentColor(QColor color);

    void changePenWidth(int width);
};

#endif // DRAW_H
