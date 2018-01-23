#-------------------------------------------------
#
# Project created by QtCreator 2017-03-21T10:09:56
#
#-------------------------------------------------

TEMPLATE    = app
TARGET      = QtScriptBasicsDemo

QT          += core gui script

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES     += \
    main.cpp \
    mainwidget.cpp

HEADERS     += \
    mainwidget.h

FORMS       += \
    mainwidget.ui

RESOURCES   += \
    resources/resources.qrc

VPATH = $$INCLUDEPATH
    
