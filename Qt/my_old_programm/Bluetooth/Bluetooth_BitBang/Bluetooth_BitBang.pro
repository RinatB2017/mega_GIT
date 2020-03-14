#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Bluetooth_BitBang

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/bluetooth_bitbang_mainbox \
    $$PWD/src/bluetooth_bitbang_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

#DEFINES += NO_LOG

HEADERS += \
    bluetooth_bitbang_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    bluetooth_bitbang_mainbox.cpp \
    main.cpp

FORMS += bluetooth_bitbang_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
