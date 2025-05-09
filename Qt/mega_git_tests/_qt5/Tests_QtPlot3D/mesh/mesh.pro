#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = mesh

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

SOURCES = \
    src/main.cpp \
    src/meshmainwindow.cpp \
    src/colormapreader.cpp \
    src/lightingdialog.cpp

HEADERS = \
    src/meshmainwindow.h \
    src/functions.h \
    src/colormapreader.h \
    src/lightingdialog.h

FORMS   = \
    src/meshmainwindowbase.ui \
    src/lightingdialogbase.ui

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}
include ($$LIB_PATH2/qwtplot3d/qwtplot3d.pri)

include ($$LIB_PATH/turbo.pri)

DESTDIR = ../bin
