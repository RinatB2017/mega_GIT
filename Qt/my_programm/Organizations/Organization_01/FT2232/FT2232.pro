#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = FT2232

unix {
    DEPENDPATH  += /usr/include/libftdi1
    DEPENDPATH  += /usr/include/libusb-1.0
}
DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ft2232_mainbox \
    $$PWD/src/ft2232_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_TESTBAR_GRAPHER
DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += RS232_SEND
DEFINES += SERIAL
DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

HEADERS += \
    defines.hpp \
    version.hpp \
    ft2232_mainbox.hpp \
    sleeper.h

SOURCES += \
    ft2232_mainbox.cpp \
    main.cpp

FORMS   += ft2232_mainbox.ui

unix {
    LIBS    += -lftdi1
    LIBS    += -lusb-1.0
    LIBS    += /opt/lib64/libftd2xx.so
}

win32 {
    RC_ICONS = ico/computer.ico
    include (./src/ftdi/ftdi.pri)
}

LIB_PATH  = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/QHexEdit_QT5/QHexEdit_QT5.pri)
include ($$LIB_PATH/time/time.pri)

include ($$LIB_PATH2/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH2/serial5/serial5.pri)
include ($$LIB_PATH2/qwt/qwt.pri)

VERSION_HEADER = src/version.hpp
include ($$LIB_PATH/auto_inc_version.pri)

include (src/devices/devices.pri)

include ($$LIB_PATH2/ft2232h/ft2232h.pri)

VPATH = $$INCLUDEPATH
