#ifndef ELLIPSE_SELECT_H
#define ELLIPSE_SELECT_H

#include <QDialog>

namespace Ui {
class ellipse_select;
}

class ellipse_select : public QDialog
{
    Q_OBJECT

public:
    explicit ellipse_select(QWidget *parent = nullptr);
    ~ellipse_select();

private:
    Ui::ellipse_select *ui;

signals:
    void drawEllipseByPara(int xc,int rx,int yc,int ry);
private slots:
    void on_parameter_clicked();
};

#endif // ELLIPSE_SELECT_H
