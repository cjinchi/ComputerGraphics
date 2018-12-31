#include "curve_info.h"
#include "ui_curve_info.h"

curve_info::curve_info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::curve_info)
{
    ui->setupUi(this);
}

curve_info::~curve_info()
{
    delete ui;
}
