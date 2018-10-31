#include "line_select.h"
#include "ui_line_select.h"
#include <QDebug>
#include <QRegExpValidator>
#include <QMessageBox>

line_select::line_select(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::line_select)
{
    ui->setupUi(this);

    QRegExp regx("[0-9\\-][0-9]+$");
    QValidator *validator_a = new QRegExpValidator(regx,ui->a);
    ui->a->setValidator(validator_a);
    QValidator *validator_b = new QRegExpValidator(regx,ui->b);
    ui->b->setValidator(validator_b);
    QValidator *validator_c = new QRegExpValidator(regx,ui->c);
    ui->c->setValidator(validator_c);

    ui->a->setText("2");
    ui->b->setText("1");
    ui->c->setText("-500");
}

line_select::~line_select()
{
    delete ui;
}


void line_select::on_drag_clicked()
{
    emit drawLineByDrag();
    this->close();
}

void line_select::on_parameter_clicked()
{
    QString a_str = ui->a->text();
    QString b_str = ui->b->text();
    QString c_str = ui->c->text();
    if(a_str.length()==0||b_str.length()==0||c_str.length()==0)
    {
        QMessageBox emptyErrorMsg;
        emptyErrorMsg.setFixedSize(400,200);
        emptyErrorMsg.setWindowTitle(tr("错误"));
        emptyErrorMsg.setText(tr("参数不可为空!"));
        emptyErrorMsg.setModal(true);
        emptyErrorMsg.exec();
        return;
    }
    else if(a_str=="-"||b_str=="-"||c_str=="-")
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
        emit drawLineByPara(a_str.toInt(),b_str.toInt(),c_str.toInt());
        this->close();
    }
}
