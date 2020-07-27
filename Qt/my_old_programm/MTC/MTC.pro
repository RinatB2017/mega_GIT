#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = MTC

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232_FIXED_SIZE

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include (src/mtc_mainbox/mtc_mainbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
