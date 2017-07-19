#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = emrichments

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

SOURCES = \
    src/main.cpp \
    src/enrichmentmainwindow.cpp \
    src/enrichments.cpp

HEADERS = \
    src/enrichmentmainwindow.h \
    src/enrichments.h

FORMS   = \
    src/enrichmentmainwindowbase.ui

LIB_PATH = "../../../../lib"

include ($$LIB_PATH/qwtplot3d/qwtplot3d.pri)
include ($$LIB_PATH/turbo.pri)

DESTDIR = ../bin
