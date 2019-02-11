#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Parport

FOLDER  = organizations/Organization_00

DEPENDPATH  += $$PWD/src
INCLUDEPATH += $$PWD/src

DEFINES += FAKE_DATADIR
DEFINES += LEDPANEL_ON

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += defines.hpp
SOURCES += main.cpp

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH  = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)

include ($$LIB_PATH2/leds/leds.pri)
include ($$LIB_PATH2/parport/parport.pri)
include ($$LIB_PATH2/parport/parportbox.pri)

include ($$LIB_PATH/mywidgets/mywidget.pri)

VPATH = $$INCLUDEPATH
