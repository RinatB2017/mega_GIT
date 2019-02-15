#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = GY_521

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/gy521_mainbox \
    $$PWD/src/gy521_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

HEADERS += \
    gy521_mainbox.hpp \
    defines.hpp \
    version.hpp \

SOURCES += \
    gy521_mainbox.cpp \
    main.cpp

FORMS += gy521_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
