#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Oscilloscope

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/oscilloscope_mainbox \
    $$PWD/src/oscilloscope_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON

HEADERS += \
    oscilloscope_mainbox.hpp \
    defines.hpp

SOURCES += \
    oscilloscope_mainbox.cpp \
    main.cpp

FORMS   += oscilloscope_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
