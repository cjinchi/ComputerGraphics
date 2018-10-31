#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stable.h"
#include "config.h"
#include "state.h"
#include "line_select.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionLine_triggered();

    void on_actionCircle_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
