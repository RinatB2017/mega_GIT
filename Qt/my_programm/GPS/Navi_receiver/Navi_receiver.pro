#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Navi_receiver

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/navi_receiver_mainbox \
    $$PWD/src/navi_receiver_mainbox/ui
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
    navi_receiver_mainbox.hpp

SOURCES += \
    navi_receiver_mainbox.cpp \
    main.cpp

FORMS   += navi_receiver_mainbox.ui

RESOURCES += doc/test_data.qrc

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include ($$LIB_PATH2/proto_NMEA_0183/proto_NMEA_0183.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
