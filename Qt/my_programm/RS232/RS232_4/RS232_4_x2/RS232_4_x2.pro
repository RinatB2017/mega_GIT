#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = RS232_4_x2

include (conf/conf.pri)

FOLDER      = RS232

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232_LOG
DEFINES += RS232_SEND
#DEFINES += RS232_FIXED_SIZE

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_LOG

SOURCES += main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial4/serial4.pri)

VPATH = $$INCLUDEPATH
