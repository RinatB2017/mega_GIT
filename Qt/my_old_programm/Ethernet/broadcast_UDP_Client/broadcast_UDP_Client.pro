#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = broadcast_UDP_Client

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/broadcast_udp_client_mainbox \
    $$PWD/src/broadcast_udp_client_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    broadcast_udp_client_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    broadcast_udp_client_mainbox.cpp \
    main.cpp

FORMS   += broadcast_udp_client_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

VERSION_HEADER = src/version.hpp
include ($$LIB_PATH/auto_inc_version.pri)

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/ethernet/udp_client.pri)

VPATH = $$INCLUDEPATH
