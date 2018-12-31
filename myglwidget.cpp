#include "myglwidget.h"
#include <QFileDialog>

MyGLWidget::MyGLWidget()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open OFF"), "", tr("OFF Files (*.off)"));
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"file error";
    }

    QTextStream in(&file);
    in.readLine();
    QString info = in.readLine();
    QStringList infos = info.split(" ");
    int pointNum = infos.at(0).toInt();
    int faceNum = infos.at(1).toInt();
    qDebug()<<pointNum<<" "<<faceNum;
    for(int i=0;i<pointNum;i++){
        info = in.readLine();
        infos = info.split(" ");
        QVector<float> oneLine;
        for(int j=0;j<infos.size();j++)
            oneLine.push_back(infos.at(j).toFloat());
        points.push_back(oneLine);
    }

    for(int i=0;i<faceNum;i++){
        info = in.readLine();
        infos = info.split(" ");
        QVector<int> oneLine;
        for(int j=1;j<infos.size();j++)
            oneLine.push_back(infos.at(j).toInt());
        faces.push_back(oneLine);
    }
}

void MyGLWidget::initializeGL()                         //Init OpenGL
{
    glClearColor(0.0, 0.0, 0.0, 0.0);                   //Black
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void MyGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glScalef(1.5,1.5,1.5);
    gluPerspective(50,2.1,0.5,3000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,-1,-1,0,0,0,0,0,-1);
    glColor3f(0.3f,0.7f,1.0f);
}


void MyGLWidget::paintGL()                              //draw
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int i=0;i<faces.size();i++){
        glBegin(GL_POLYGON);
        for(int j=0;j<faces[i].size();j++){
            glVertex3f(points[faces[i][j]][0],points[faces[i][j]][1],points[faces[i][j]][2]);
        }
        glEnd();
    }
}
