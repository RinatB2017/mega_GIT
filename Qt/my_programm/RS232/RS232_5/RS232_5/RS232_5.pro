#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = RS232_5

include (conf/conf.pri)

FOLDER      = RS232

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232_SEND
DEFINES += RS232_LOG

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/RS232.ico
}

LIB_PATH  = "$$PWD/../../../../lib"
LIB_PATH2 = "$$PWD/../../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include (src/rs232_5_mainbox/rs232_5_mainbox.pri)

lessThan(QT_MAJOR_VERSION, 5) {
    error (Only Qt5)
}

RESOURCES += \
    ico/icons.qrc

VPATH = $$INCLUDEPATH

