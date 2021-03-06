#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stable.h"
#include "config.h"
#include "state.h"
#include "line_select.h"
#include <QAction>
#include <QSlider>
#include <QColorDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionLine_triggered();

    void on_actionCircle_triggered();

    void on_actionEllipse_triggered();

    void drawLineByDrag();

    void drawLineByPara(int a,int b,int c);

    void drawCircleByDrag();

    void drawCircleByPara(int x0,int y0,int r);

    void drawEllipseByDrag();

    void drawEllipseByPara(int xc,int rx,int yc,int ry);

    void on_actionClear_triggered();

    void on_actionSave_as_triggered();

    void on_actioninfo_triggered();

    void on_actionpolygon_triggered();

    void on_actionfill_triggered();

    void on_actioncurve_triggered();

    void on_actionclip_triggered();

    void on_rotate_valueChanged(int value);

    void on_pushButton_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_action3D_triggered();

private:
    Ui::MainWindow *ui;
    QString help_text;
    QSlider rotate;
    QSlider zoom;
    QColorDialog qcd;


signals:
    void toDrawShapeByDrag(shape shape_to_draw);

    void toDrawLineByPara(int a,int b,int c);

    void toDrawCircleByPara(int x0,int y0,int r);

    void toDrawEllipseByPara(int xc,int rx,int yc,int ry);

    void toFill();

    void clearPixmap();

    void savePixmap(QString path);

    void changeColor(QColor color);

    void changePenWidth(int width);

public slots:
    void setSlidersVisible(bool enable);
};

#endif // MAINWINDOW_H
