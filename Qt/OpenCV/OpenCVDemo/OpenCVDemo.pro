#-------------------------------------------------
#
# Project created by QtCreator 2017-03-30T08:35:23
#
#-------------------------------------------------

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT          += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = OpenCVDemo
TEMPLATE    = app

SOURCES     += \
    main.cpp \
    mainwidget.cpp

HEADERS     += mainwidget.h

FORMS       += mainwidget.ui

#LIBS += -lopencv_core \
#        -lopencv_imgproc \
#        -lopencv_highgui \
#        -lopencv_objdetect \
#        -lopencv_imgcodecs

PKGCONFIG   += opencv
CONFIG      += link_pkgconfig
        
VPATH = $$INCLUDEPATH
