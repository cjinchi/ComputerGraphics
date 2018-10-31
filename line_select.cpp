#include "line_select.h"
#include "ui_line_select.h"
#include <QDebug>

line_select::line_select(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::line_select)
{
    ui->setupUi(this);
    this->setFixedSize(800,300);
}

line_select::~line_select()
{
    delete ui;
}

void line_select::on_pushButton_clicked()
{
    //
    qDebug()<<"tell draw.cpp to change state";

}

void line_select::on_pushButton_2_clicked()
{
    qDebug()<<"tell draw.cpp to change state";
}
