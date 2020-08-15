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

LIB_PATH = "$$PWD/../../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../../Qt/lib2"

include ($$LIB_PATH2/qwtplot3d/qwtplot3d.pri)

include ($$LIB_PATH/turbo.pri)

DESTDIR = ../bin
