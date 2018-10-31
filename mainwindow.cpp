#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "line_select.h"
#include "circle_select.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLine_triggered()
{
    line_select x;
    x.setModal(true);
    x.show();
    x.exec();

}

void MainWindow::on_actionCircle_triggered()
{
    circle_select x;
    x.setModal(true);
    x.show();
    x.exec();
}
