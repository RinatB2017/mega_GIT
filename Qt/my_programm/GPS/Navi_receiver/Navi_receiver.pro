#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Navi_receiver

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

RESOURCES += doc/test_data.qrc

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include ($$LIB_PATH2/proto_NMEA_0183/proto_NMEA_0183.pri)

include (src/navi_receiver_mainbox/navi_receiver_mainbox.pri)

VPATH = $$INCLUDEPATH
