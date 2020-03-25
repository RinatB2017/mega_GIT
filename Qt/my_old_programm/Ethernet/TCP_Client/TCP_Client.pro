#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = TCP_Client

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/tcp_client_mainbox \
    $$PWD/src/tcp_client_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    tcp_client_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    tcp_client_mainbox.cpp \
    main.cpp

FORMS   += tcp_client_mainbox.ui

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

VPATH = $$INCLUDEPATH
