#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T08:58:07
#
#-------------------------------------------------

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SyntaxHighlighterDemo
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwidget.cpp

HEADERS  += mainwidget.h

FORMS    += mainwidget.ui

VPATH = $$INCLUDEPATH
