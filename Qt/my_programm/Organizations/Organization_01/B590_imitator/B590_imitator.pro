#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = B590_imitator

FOLDER      = organizations/Organization_01

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/b590_imitator \
    $$PWD/src/b590_imitator/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH  = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/time/time.pri)

include ($$LIB_PATH2/connection/connection.pri)
include ($$LIB_PATH2/crc/crc.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include (../lib/powersupply/find_powersupply/find_powersupply.pri)
include (../lib/powersupply/powersupply_b590/powersupply_b590.pri)
include (../lib/powersupply/fram_b590/fram_b590.pri)
include (../lib/b590_imitator/b590_imitator.pri)
include (../lib/upacket/upacket.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
#message ($$QT)
