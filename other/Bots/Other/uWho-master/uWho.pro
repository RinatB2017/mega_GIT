#-------------------------------------------------
#
# Project created by QtCreator 2015-02-20T11:22:57
#
#-------------------------------------------------

QT       += core gui

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/ui \
    $$PWD/images
INCLUDEPATH = $$DEPENDPATH

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = uWho
TEMPLATE    = app

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    webget.cpp

HEADERS  += \
    mainwindow.h \
    webget.h

FORMS    += \
    mainwindow.ui \
    webget.ui

RESOURCES += \
    resources.qrc

PKGCONFIG   += opencv
CONFIG      += link_pkgconfig

VPATH = $$INCLUDEPATH
