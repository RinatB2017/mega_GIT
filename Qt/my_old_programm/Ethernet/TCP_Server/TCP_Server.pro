#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = TCP_Server

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/tcp_server_mainbox \
    $$PWD/src/tcp_server_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    tcp_server_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    tcp_server_mainbox.cpp \
    main.cpp

FORMS   += tcp_server_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH  = "../../../lib"
LIB_PATH2 = "../../../lib2"

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH2/ethernet/tcp_server.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)

VPATH = $$INCLUDEPATH
