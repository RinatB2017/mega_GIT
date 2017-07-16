#-------------------------------------------------
#
# Project created by QtCreator 2015-09-23T09:50:35
#
#-------------------------------------------------

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT          += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = qtelnet
TEMPLATE    = app

SOURCES     += \
    main.cpp \
    mainwindow.cpp \
    qtelnet.cpp

HEADERS     += \
    mainwindow.h \
    qtelnet.h

LIBS        += -ltelnet

FORMS       += mainwindow.ui

VPATH = $$INCLUDEPATH
