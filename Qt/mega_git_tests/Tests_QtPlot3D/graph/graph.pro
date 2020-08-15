#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = graph

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

SOURCES = src/graph.cpp

LIB_PATH = "$$PWD/../../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../../Qt/lib2"

include ($$LIB_PATH2/qwtplot3d/qwtplot3d.pri)

include ($$LIB_PATH/turbo.pri)

DESTDIR = ../bin
