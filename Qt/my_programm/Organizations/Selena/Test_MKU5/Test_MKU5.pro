#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_MKU5

FOLDER  = organizations/Selena

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui \
    $$PWD/src/debug_frame
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

DEFINES += RS232
DEFINES += RS232_LOG
#DEFINES += RS232_SEND

DEFINES += DEBUG


DEFINES += NO_LOG

HEADERS += \
    src/defines.hpp \
    src/version.hpp \
    src/mainbox/mainbox.hpp

SOURCES += \
    src/mainbox/mainbox.cpp \
    src/main.cpp

contains(DEFINES, DEBUG_FRAME) {
    HEADERS += src/debug_frame/debugframe.hpp
    SOURCES += src/debug_frame/debugframe.cpp
}

FORMS += src/mainbox/ui/mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../../lib"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/ethernet/tcp_client.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/serial5/serial5.pri)
include ($$LIB_PATH/turbo.pri)
include (test.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

#include ($$LIB_PATH/ethernet/fake/fake.pri)

VPATH = $$INCLUDEPATH
