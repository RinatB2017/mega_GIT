#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T23:52:59
#
#-------------------------------------------------

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH


QT       += core gui svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SvgExample
TEMPLATE = app


SOURCES += \
    main.cpp\
    mainwindow.cpp \
    svgreader.cpp

HEADERS  += \
    mainwindow.h \
    svgreader.h

FORMS    += mainwindow.ui

VPATH = $$INCLUDEPATH
