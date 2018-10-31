#include "circle_select.h"
#include "ui_circle_select.h"
#include <QRegExpValidator>
#include <QMessageBox>
circle_select::circle_select(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::circle_select)
{
    ui->setupUi(this);

    QRegExp regx("[0-9\\-][0-9]+$");
    QValidator *validator_x0 = new QRegExpValidator(regx,ui->x0);
    ui->x0->setValidator(validator_x0);
    QValidator *validator_y0 = new QRegExpValidator(regx,ui->y0);
    ui->y0->setValidator(validator_y0);
    QValidator *validator_r = new QRegExpValidator(regx,ui->r);
    ui->r->setValidator(validator_r);

    ui->x0->setText("400");
    ui->y0->setText("400");
    ui->r->setText("200");
}

circle_select::~circle_select()
{
    delete ui;
}

void circle_select::on_parameter_clicked()
{
    QString x0_str = ui->x0->text();
    QString y0_str = ui->y0->text();
    QString r_str = ui->r->text();
    if(x0_str.length()==0||y0_str.length()==0||r_str.length()==0)
    {
        QMessageBox emptyErrorMsg;
        emptyErrorMsg.setFixedSize(400,200);
        emptyErrorMsg.setWindowTitle(tr("错误"));
        emptyErrorMsg.setText(tr("参数不可为空!"));
        emptyErrorMsg.setModal(true);
        emptyErrorMsg.exec();
        return;
    }
    else if(x0_str=="-"||y0_str=="-"||r_str=="-")
    {
        QMessageBox numErrorMsg;
        numErrorMsg.setFixedSize(400,200);
        numErrorMsg.setWindowTitle(tr("错误"));
        numErrorMsg.setText(tr("参数格式错误!"));
        numErrorMsg.setModal(true);
        numErrorMsg.exec();
        return;
    }
    else
    {
        emit drawCircleByPara(x0_str.toInt(),y0_str.toInt(),r_str.toInt());
        this->close();
    }
}

void circle_select::on_drag_clicked()
{
    emit drawCircleByDrag();
    this->close();
}
