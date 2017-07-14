#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = FT2232

unix {
DEPENDPATH  += /usr/include/libftdi1
}
DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_TESTBAR_GRAPHER
DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += FLAG_RESIZE
DEFINES += RS232_SEND
DEFINES += SERIAL
DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp \
    sleeper.h

SOURCES += \
    main.cpp \
    mainbox.cpp

FORMS += mainbox.ui

unix {
    LIBS    += -lftdi1
    LIBS    += -lusb
    LIBS    += /opt/lib64/libftd2xx.so
}

win32 {
    RC_FILE = ico/myapp.rc
    include (./src/ftdi/ftdi.pri)
}

LIB_PATH = "../../lib"

include ($$LIB_PATH/qtsingleapplication/qtsingleapplication.pri)
include ($$LIB_PATH/mysplashscreen/mysplashscreen.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/QHexEdit/qhexedit.pri)
include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)

include ($$LIB_PATH/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH/csvreader/csvreader.pri)
include ($$LIB_PATH/serial/serial.pri)
include ($$LIB_PATH/qwt/qwt.pri)

VERSION_HEADER = src/version.hpp
include ($$LIB_PATH/auto_inc_version.pri)

include (src/devices/devices.pri)
include (src/ft2232h/ft2232h.pri)

VPATH = $$INCLUDEPATH
