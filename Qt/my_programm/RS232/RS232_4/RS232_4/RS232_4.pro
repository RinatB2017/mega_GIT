#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = RS232_4

include (conf/conf.pri)

FOLDER      = RS232

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/rs232_4_mainbox \
    $$PWD/src/rs232_4_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232_FIXED_SIZE
DEFINES += RS232_SEND
#DEFINES += RS232_LOG

HEADERS += \
    rs232_4_mainbox.hpp \
    defines.hpp

SOURCES += \
    rs232_4_mainbox.cpp \
    main.cpp

FORMS   += rs232_4_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../../lib"
LIB_PATH2 = "$$PWD/../../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial4/serial4.pri)

VPATH = $$INCLUDEPATH
