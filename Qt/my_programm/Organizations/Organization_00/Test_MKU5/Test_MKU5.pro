#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_MKU5

FOLDER  = organizations/Organization_00

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_mku5_mainbox \
    $$PWD/src/test_mku5_mainbox/ui \
    $$PWD/src/debug_frame
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

DEFINES += RS232
DEFINES += RS232_LOG
#DEFINES += RS232_SEND

DEFINES += DEBUG
DEFINES += NO_LOG

HEADERS += \
    defines.hpp \
    version.hpp \
    test_mku5_mainbox.hpp

SOURCES += \
    test_mku5_mainbox.cpp \
    main.cpp

contains(DEFINES, DEBUG_FRAME) {
    HEADERS += src/debug_frame/debugframe.hpp
    SOURCES += src/debug_frame/debugframe.cpp
}

FORMS   +=test_mku5_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH  = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)

include ($$LIB_PATH2/ethernet/tcp_client.pri)
include ($$LIB_PATH2/serial5/serial5.pri)
include (test.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

#include ($$LIB_PATH/ethernet/fake/fake.pri)

VPATH = $$INCLUDEPATH
