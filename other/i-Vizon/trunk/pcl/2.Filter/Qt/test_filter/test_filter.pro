#-------------------------------------------------
#
# Project created by QtCreator 2014-04-26T17:41:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_filter
TEMPLATE = app

include(../../../wrapper/pclWrapper.pri)

SOURCES += main.cpp\
        filterwindow.cpp

HEADERS  += filterwindow.h \
    ../../../wrapper/inc/iv_pcl_data_types.h

FORMS    += filterwindow.ui
