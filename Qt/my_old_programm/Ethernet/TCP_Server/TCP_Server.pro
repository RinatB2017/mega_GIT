#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = TCP_Server

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/ethernet/tcp_server/tcp_server.pri)

include (src/tcp_server_mainbox/tcp_server_mainbox.pri)

VPATH = $$INCLUDEPATH
