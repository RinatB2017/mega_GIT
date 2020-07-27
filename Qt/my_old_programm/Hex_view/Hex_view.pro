#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Hex_view

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

#DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON

#DEFINES += NO_LOG

HEADERS += \
    defines.hpp \
    version.hpp \

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include ($$LIB_PATH2/QHexEdit_QT5/QHexEdit_QT5.pri)

include (src/hex_view_mainbox/hex_view_mainbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
