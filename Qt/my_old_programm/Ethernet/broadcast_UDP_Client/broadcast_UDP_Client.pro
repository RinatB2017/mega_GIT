#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = broadcast_UDP_Client

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

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

VERSION_HEADER = $$PWD/src/version.hpp
include ($$LIB_PATH/auto_inc_version.pri)

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/ethernet/udp_client/udp_client.pri)

include (src/broadcast_udp_client_mainbox/broadcast_udp_client_mainbox.pri)

VPATH = $$INCLUDEPATH
