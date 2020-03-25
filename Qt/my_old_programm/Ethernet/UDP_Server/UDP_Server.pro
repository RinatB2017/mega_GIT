#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = UDP_Server

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/udp_server_mainbox \
    $$PWD/src/udp_server_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += network

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    udp_server_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    udp_server_mainbox.cpp \
    main.cpp

FORMS   += udp_server_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "$$PWD/../../../lib"

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)

VPATH = $$INCLUDEPATH
