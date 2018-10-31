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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::line_select *ui;
};

#endif // LINE_SELECT_H
