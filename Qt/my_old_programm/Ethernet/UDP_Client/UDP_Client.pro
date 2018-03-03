#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = UDP_Client

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/udp_client_mainbox \
    $$PWD/src/udp_client_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += network

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    udp_client_mainbox.hpp

SOURCES += \
    udp_client_mainbox.cpp \
    main.cpp

FORMS   += udp_client_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)

include (src/worker/worker.pri)

VPATH = $$INCLUDEPATH
