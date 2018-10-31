#include "ellipse_select.h"
#include "ui_ellipse_select.h"
#include <QRegExpValidator>
#include <QMessageBox>

ellipse_select::ellipse_select(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ellipse_select)
{
    ui->setupUi(this);

    QRegExp regx("[0-9\\-][0-9]+$");
    QValidator *validator_xc = new QRegExpValidator(regx,ui->xc);
    ui->xc->setValidator(validator_xc);
    QValidator *validator_rx = new QRegExpValidator(regx,ui->rx);
    ui->rx->setValidator(validator_rx);
    QValidator *validator_yc = new QRegExpValidator(regx,ui->yc);
    ui->yc->setValidator(validator_yc);
    QValidator *validator_ry = new QRegExpValidator(regx,ui->ry);
    ui->ry->setValidator(validator_ry);

    ui->xc->setText("600");
    ui->rx->setText("200");
    ui->yc->setText("400");
    ui->ry->setText("100");
}

ellipse_select::~ellipse_select()
{
    delete ui;
}

void ellipse_select::on_parameter_clicked()
{
    QString xc_str = ui->xc->text();
    QString rx_str = ui->rx->text();
    QString yc_str = ui->yc->text();
    QString ry_str = ui->ry->text();

    if(xc_str.length()==0||rx_str.length()==0||yc_str.length()==0||ry_str.length()==0)
    {
        QMessageBox emptyErrorMsg;
        emptyErrorMsg.setFixedSize(400,200);
        emptyErrorMsg.setWindowTitle(tr("错误"));
        emptyErrorMsg.setText(tr("参数不可为空!"));
        emptyErrorMsg.setModal(true);
        emptyErrorMsg.exec();
        return;
    }
    else if(xc_str=="-"||rx_str=="-"||yc_str=="-"||ry_str=="-")
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
        emit drawEllipseByPara(xc_str.toInt(),rx_str.toInt(),yc_str.toInt(),ry_str.toInt());
        this->close();
    }
}
