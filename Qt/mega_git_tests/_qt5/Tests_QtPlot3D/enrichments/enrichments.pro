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

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH2/qwtplot3d/qwtplot3d.pri)

include ($$LIB_PATH/turbo.pri)

DESTDIR = ../bin
