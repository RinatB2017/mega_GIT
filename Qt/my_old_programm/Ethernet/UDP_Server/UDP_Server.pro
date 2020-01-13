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

DEFINES += SAVE_WIDGETS_LINEEDIT
DEFINES += SAVE_WIDGETS_SPINBOX

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

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)

VPATH = $$INCLUDEPATH
