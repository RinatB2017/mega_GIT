#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Navi_old

FOLDER  = organizations/Selena

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += FLAG_RESIZE
DEFINES += WOODSTYLE

DEFINES += RS232_LOG
#DEFINES += RS232_SEND

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

#DEFINES += TEST_NAVI

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS += mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/serial/serial.pri)
include ($$LIB_PATH/proto_NMEA_0183/proto_NMEA_0183.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

include (navigator/navigator.pri)

VPATH = $$INCLUDEPATH
