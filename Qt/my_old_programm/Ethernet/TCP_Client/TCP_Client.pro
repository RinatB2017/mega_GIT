#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = TCP_Client

FOLDER  = old_programm

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

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/ethernet/tcp_client.pri)
include ($$LIB_PATH2/widgets/ipv4/ipv4.pri)

#include ($$LIB_PATH/ethernet/fake/fake.pri)

include (src/tcp_client_mainbox/tcp_client_mainbox.pri)

VPATH = $$INCLUDEPATH
