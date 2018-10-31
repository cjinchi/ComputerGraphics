#ifndef LINE_SELECT_H
#define LINE_SELECT_H

#include <QDialog>

namespace Ui {
class line_select;
}

class line_select : public QDialog
{
    Q_OBJECT

public:
    explicit line_select(QWidget *parent = nullptr);
    ~line_select();

private slots:

    void on_drag_clicked();

    void on_parameter_clicked();

private:
    Ui::line_select *ui;

signals:
    void drawLineByPara(int a,int b,int c);
    void drawLineByDrag();
};

#endif // LINE_SELECT_H
