#-------------------------------------------------
#
# Project created by QtCreator 2015-12-03T20:24:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PaintFigure
TEMPLATE = app

SOURCES += \
    src/main.cpp\
    src/mainwindow.cpp \
    src/romb.cpp \
    src/paintscene.cpp \
    src/figure.cpp \
    src/triangle.cpp \
    src/square.cpp

HEADERS  += \
    src/mainwindow.h \
    src/romb.h \
    src/paintscene.h \
    src/figure.h \
    src/triangle.h \
    src/square.h

FORMS    += src/ui/mainwindow.ui
