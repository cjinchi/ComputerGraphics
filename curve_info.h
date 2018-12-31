#ifndef CURVE_INFO_H
#define CURVE_INFO_H

#include <QDialog>

namespace Ui {
class curve_info;
}

class curve_info : public QDialog
{
    Q_OBJECT

public:
    explicit curve_info(QWidget *parent = nullptr);
    ~curve_info();

private:
    Ui::curve_info *ui;
};

#endif // CURVE_INFO_H
