#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "line_select.h"
#include "circle_select.h"
#include "ellipse_select.h"
#include<QDebug>
#include<QFileDialog>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);

    help_text = tr("目前使用Qt的坐标系统，绘图区域左上角为坐标原点，从左到右为x轴正方向，从上到下为y轴正方向.建议直接使用方程默认参数进行测试.");

//    QMessageBox msgbox;
//    msgbox.setFixedSize(800,600);
//    msgbox.setWindowTitle(tr("说明"));
//    msgbox.setText(help_text);
//    msgbox.setModal(true);
//    msgbox.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLine_triggered()
{
    line_select x;
    QObject::connect(&x,SIGNAL(drawLineByDrag()),this,SLOT(drawLineByDrag()));
    QObject::connect(&x,SIGNAL(drawLineByPara(int,int,int)),this,SLOT(drawLineByPara(int,int,int)));
    x.setModal(true);
    x.show();
    x.exec();

}

void MainWindow::on_actionCircle_triggered()
{
    circle_select x;
    QObject::connect(&x,SIGNAL(drawCircleByDrag()),this,SLOT(drawCircleByDrag()));
    QObject::connect(&x,SIGNAL(drawCircleByPara(int,int,int)),this,SLOT(drawCircleByPara(int,int,int)));
    x.setModal(true);
    x.show();
    x.exec();
}

void MainWindow::on_actionEllipse_triggered()
{
    ellipse_select x;
    QObject::connect(&x,SIGNAL(drawEllipseByPara(int,int,int,int)),this,SLOT(drawEllipseByPara(int,int,int,int)));
    QObject::connect(&x,SIGNAL(drawEllipseByDrag()),this,SLOT(drawEllipseByDrag()));
    x.setModal(true);
    x.show();
    x.exec();
}

void MainWindow::drawLineByDrag()
{
    emit toDrawShapeByDrag(LINE);
}

void MainWindow::drawLineByPara(int a,int b,int c)
{
    emit toDrawLineByPara(a,b,c);
}

void MainWindow::drawCircleByDrag()
{
    emit toDrawShapeByDrag(CIRCLE);
}

void MainWindow::drawCircleByPara(int x0,int y0,int r)
{
    emit toDrawCircleByPara(x0,y0,r);
}

void MainWindow::drawEllipseByDrag()
{
    emit toDrawShapeByDrag(ELLIPSE);
}

void MainWindow::drawEllipseByPara(int xc,int rx,int yc,int ry)
{
    emit toDrawEllipseByPara(xc,rx,yc,ry);
}

void MainWindow::on_actionClear_triggered()
{
    emit clearPixmap();
}

void MainWindow::on_actionSave_as_triggered()
{
    QString path = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png)"));
    if(!path.isEmpty())
    {
        emit savePixmap(path);
    }
}

void MainWindow::on_actioninfo_triggered()
{
    QMessageBox msgbox;
    msgbox.setFixedSize(800,600);
    msgbox.setWindowTitle(tr("说明"));
    msgbox.setText(help_text);
    msgbox.setModal(true);
    msgbox.exec();
}


void MainWindow::on_actionpolygon_triggered()
{
    emit toDrawShapeByDrag(POLYGON);
}

void MainWindow::on_actionfill_triggered()
{
    emit toFill();
}
