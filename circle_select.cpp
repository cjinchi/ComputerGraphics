#include "circle_select.h"
#include "ui_circle_select.h"

circle_select::circle_select(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::circle_select)
{
    ui->setupUi(this);
}

circle_select::~circle_select()
{
    delete ui;
}
