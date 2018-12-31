#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <gl/GLU.h>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class MyGLWidget : public QGLWidget
{
public:
    MyGLWidget();

private:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    QVector<QVector<float>> points;
    QVector<QVector<int>> faces;
};

#endif // MYGLWIDGET_H
