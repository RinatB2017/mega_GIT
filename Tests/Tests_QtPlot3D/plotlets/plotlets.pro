#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = plotlets

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

SOURCES = \
    src/main.cpp \
    src/mainwindow.cpp \
    src/configframe.cpp

HEADERS = \
    src/mainwindow.h \
    src/functions.h \
    src/configframe.h
FORMS   = \
    src/mainwindowbase.ui \
    src/configframebase.ui

LIB_PATH = "../../../../lib"

include ($$LIB_PATH/qwtplot3d/qwtplot3d.pri)
include ($$LIB_PATH/turbo.pri)

DESTDIR = ../bin
