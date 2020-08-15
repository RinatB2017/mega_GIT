#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = axes

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

SOURCES = \
    src/main.cpp \
    src/axesmainwindow.cpp

HEADERS = \
    src/axesmainwindow.h \
    src/axes.h

FORMS   = \
    src/axesmainwindowbase.ui

LIB_PATH = "$$PWD/../../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../../Qt/lib2"

include ($$LIB_PATH2/qwtplot3d/qwtplot3d.pri)

include ($$LIB_PATH/turbo.pri)

DESTDIR = ../bin
