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

private:
    Ui::circle_select *ui;
};

#endif // CIRCLE_SELECT_H
