#-------------------------------------------------
#
# Project created by QtCreator 2015-11-01T22:56:47
#
#-------------------------------------------------

QT       += core gui

TARGET = Tetris
TEMPLATE = app

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH


SOURCES += \
    src/main.cpp\
    src/tetrisview.cpp \
    src/tetrismodel.cpp \
    src/tetriscontroller.cpp

HEADERS  += \
    src/tetrisview.h \
    src/tetrismodel.h \
    src/tetriscontroller.h

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

