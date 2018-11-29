#include "polygon_info.h"
#include "ui_polygon_info.h"

polygon_info::polygon_info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::polygon_info)
{
    ui->setupUi(this);
}

polygon_info::~polygon_info()
{
    delete ui;
}
