#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = broadcast_UDP_Client

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column


DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS += mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"

VERSION_HEADER = src/version.hpp
include ($$LIB_PATH/auto_inc_version.pri)

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)
include ($$LIB_PATH/ethernet/udp_client.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)

VPATH = $$INCLUDEPATH
