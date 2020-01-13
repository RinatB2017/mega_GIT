#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = RS232_4_x2

FOLDER      = RS232

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232_LOG
DEFINES += RS232_SEND

#DEFINES += RS232_FIXED_SIZE
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON


DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_LOG

SOURCES += main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../../lib"
LIB_PATH2 = "$$PWD/../../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial4/serial4.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
