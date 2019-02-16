#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = SIM900

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/sim900_mainbox \
    $$PWD/src/sim900_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#DEFINES += RS232_FIXED_SIZE
DEFINES += RS232_SEND
DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8
#DEFINES += NO_LOG

HEADERS += \
    sim900_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    sim900_mainbox.cpp \
    main.cpp

FORMS += sim900_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
