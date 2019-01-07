#-------------------------------------------------
#
# Project created by QtCreator 2018-10-26T19:22:40
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += opengl32.lib glu32.lib

TARGET = ComputerGraphics
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    draw.cpp \
    line_select.cpp \
    circle_select.cpp \
    ellipse_select.cpp \
    polygon_info.cpp \
    curve_info.cpp \
    myglwidget.cpp

HEADERS += \
        mainwindow.h \
    draw.h \
    stable.h \
    config.h \
    state.h \
    line_select.h \
    circle_select.h \
    ellipse_select.h \
    polygon_info.h \
    curve_info.h \
    myglwidget.h

FORMS += \
        mainwindow.ui \
    line_select.ui \
    circle_select.ui \
    ellipse_select.ui \
    polygon_info.ui \
    curve_info.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

PRECOMPILED_HEADER = stable.h
QMAKE_CXXFLAGS += /MP

RESOURCES +=

DISTFILES +=
