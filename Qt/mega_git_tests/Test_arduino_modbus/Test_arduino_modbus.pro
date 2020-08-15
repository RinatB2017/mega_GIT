#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Test_arduino_modbus

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_arduino_modbus_mainbox \
    $$PWD/src/test_arduino_modbus_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#DEFINES += RS232_FIXED_SIZE
DEFINES += RS232_SEND
DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

#DEFINES += NO_LOG

HEADERS += \
    test_arduino_modbus_mainbox.hpp \
    defines.hpp \
    version.hpp \

SOURCES += \
    test_arduino_modbus_mainbox.cpp \
    main.cpp

FORMS   += test_arduino_modbus_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serial5.pri)
include ($$LIB_PATH2/crc/crc.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
