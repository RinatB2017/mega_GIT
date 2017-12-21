#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = FT2232_old

unix {
DEPENDPATH  += /usr/include/libftdi1
}
DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui \
    $$PWD/src/i2c \
    $$PWD/src/spi \
    $$PWD/src/mcp4922 \
    $$PWD/src/hd44780 \
    $$PWD/src/bitbang \
    $$PWD/src/ad8400
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_TESTBAR_GRAPHER
DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += FLAG_RESIZE
DEFINES += RS232_SEND
DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp \
    sleeper.h \
    i2c.hpp \
    spi.hpp \
    mcp4922.hpp \
    hd44780.hpp \
    bitbang.hpp \
    ad8400.hpp

SOURCES += \
    main.cpp \
    mainbox.cpp \
    i2c.cpp \
    spi.cpp \
    mcp4922.cpp \
    hd44780.cpp \
    bitbang.cpp \
    ad8400.cpp

FORMS += mainbox.ui

unix {
    LIBS    += -lftdi1
    LIBS    += -lusb-1.0
    LIBS    += /opt/lib64/libftd2xx.so
}

win32 {
    RC_FILE = ico/myapp.rc
    include (./src/ftdi/ftdi.pri)
}

LIB_PATH = "../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/QHexEdit_QT5/QHexEdit_QT5.pri)

include ($$LIB_PATH/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH/csvreader/csvreader.pri)
include ($$LIB_PATH/serial5/serial5.pri)
include ($$LIB_PATH/qwt/qwt.pri)

VERSION_HEADER = src/version.hpp
include ($$LIB_PATH/auto_inc_version.pri)

VPATH = $$INCLUDEPATH
