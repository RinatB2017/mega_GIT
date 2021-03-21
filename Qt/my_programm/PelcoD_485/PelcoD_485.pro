#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = PelcoD_485

include (conf/conf.pri)

FOLDER      = RS232

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT  += network
QT  += multimediawidgets

DEFINES += WIN32_LEAN_AND_MEAN

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES	+= images/images.qrc

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/crc/crc.pri)

include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
#include ($$LIB_PATH2/serial5/serial5.pri)
include ($$LIB_PATH2/serial5/serialbox5/serialbox5.pri)

include ($$LIB_PATH2/power_knock/power_knock.pri)

include ($$LIB_PATH2/widgets/hexspinbox/hexspinbox.pri)
include ($$LIB_PATH2/icons/arrows.pri)

include (src/pelco_d_485_mainbox/pelco_d_485_mainbox.pri)

lessThan(QT_MAJOR_VERSION, 5) {
    error (Only Qt5)
}

VPATH = $$INCLUDEPATH
