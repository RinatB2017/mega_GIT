#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Bluetooth_6x8_matrix

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/bluetooth_6x8_matrix_mainbox \
    $$PWD/src/bluetooth_6x8_matrix_mainbox/ui
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
    version.hpp \
    bluetooth_6x8_matrix_mainbox.hpp

SOURCES += \
    bluetooth_6x8_matrix_mainbox.cpp \
    main.cpp

FORMS += bluetooth_6x8_matrix_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include (src/led/led.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
