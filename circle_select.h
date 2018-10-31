#ifndef CIRCLE_SELECT_H
#define CIRCLE_SELECT_H

#include <QDialog>

namespace Ui {
class circle_select;
}

class circle_select : public QDialog
{
    Q_OBJECT

public:
    explicit circle_select(QWidget *parent = nullptr);
    ~circle_select();

private slots:
    void on_parameter_clicked();

    void on_drag_clicked();

private:
    Ui::circle_select *ui;

signals:
    void drawCircleByDrag();
    void drawCircleByPara(int x0,int y0,int r);
};

#endif // CIRCLE_SELECT_H
