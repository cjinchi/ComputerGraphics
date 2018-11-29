#ifndef POLYGON_INFO_H
#define POLYGON_INFO_H

#include <QDialog>

namespace Ui {
class polygon_info;
}

class polygon_info : public QDialog
{
    Q_OBJECT

public:
    explicit polygon_info(QWidget *parent = nullptr);
    ~polygon_info();

private:
    Ui::polygon_info *ui;
};

#endif // POLYGON_INFO_H
