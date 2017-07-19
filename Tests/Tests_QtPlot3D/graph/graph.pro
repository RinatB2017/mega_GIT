#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = graph

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

SOURCES = src/graph.cpp

LIB_PATH = "../../../../lib"

include ($$LIB_PATH/qwtplot3d/qwtplot3d.pri)
include ($$LIB_PATH/turbo.pri)

DESTDIR = ../bin
