#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = auto_inc_version

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

CONFIG += c++11 console
CONFIG -= app_bundle
QT -= gui

SOURCES += main.cpp

LIB_PATH = "$$PWD/../../lib"

include ($$LIB_PATH/turbo.pri)

unix {
    DESTDIR = $$(HOME)/bin
}
win32 {
    DESTDIR = C:\bin
}

VPATH = $$INCLUDEPATH
