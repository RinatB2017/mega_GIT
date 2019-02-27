#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = auto_inc_version

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

SOURCES += main.cpp

LIB_PATH = "../../lib"

include ($$LIB_PATH/turbo.pri)

unix {
    DESTDIR = $$(HOME)/bin
}
win32 {
    DESTDIR = C:\bin
}

VPATH = $$INCLUDEPATH
