#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = ADNS_2610

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/adns_2610_mainbox \
    $$PWD/src/adns_2610_mainbox/ui \
    $$PWD/src/screen
INCLUDEPATH = $$DEPENDPATH

#DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    adns_2610_mainbox.hpp \
    defines.hpp \
    version.hpp \
    screen.hpp

SOURCES += \
    adns_2610_mainbox.cpp \
    main.cpp \
    screen.cpp

FORMS += adns_2610_mainbox.ui

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
