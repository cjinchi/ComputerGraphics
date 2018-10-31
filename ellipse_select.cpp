#include "ellipse_select.h"
#include "ui_ellipse_select.h"

ellipse_select::ellipse_select(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ellipse_select)
{
    ui->setupUi(this);
}

ellipse_select::~ellipse_select()
{
    delete ui;
}
