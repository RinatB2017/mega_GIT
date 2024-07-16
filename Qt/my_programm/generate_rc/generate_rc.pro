#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = generate_rc

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

CONFIG += c++11
CONFIG -= app_bundle
QT -= gui

SOURCES += main.cpp

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/turbo.pri)

unix {
    DESTDIR = $$(HOME)/bin
}
win32 {
    DESTDIR = C:\bin
}

VPATH = $$INCLUDEPATH
