#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = dynamicplot

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS = src/dynamicplot.h
SOURCES = src/dynamicplot.cpp

LIB_PATH = "../../../../lib"

include ($$LIB_PATH/qwtplot3d/qwtplot3d.pri)
include ($$LIB_PATH/turbo.pri)

DESTDIR = ../bin
