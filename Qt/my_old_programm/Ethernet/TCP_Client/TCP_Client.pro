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
DEFINES += NO_TRAYICON

#DEFINES += LOGO_GL

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

LIB_PATH  = "../../../lib"
LIB_PATH2 = "../../../lib2"

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)
include ($$LIB_PATH2/ethernet/tcp_client.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)

#include ($$LIB_PATH/ethernet/fake/fake.pri)

VPATH = $$INCLUDEPATH
