#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = PelcoD_485

FOLDER      = RS232

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/pelco_d_485_mainbox \
    $$PWD/src/pelco_d_485_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += network
QT  += multimediawidgets

#DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

DEFINES += WIN32_LEAN_AND_MEAN

HEADERS += \
    pelco_d_485_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    pelco_d_485_mainbox.cpp \
    main.cpp

FORMS   += pelco_d_485_mainbox.ui

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

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

lessThan(QT_MAJOR_VERSION, 5) {
    error (Only Qt5)
}

VPATH = $$INCLUDEPATH
