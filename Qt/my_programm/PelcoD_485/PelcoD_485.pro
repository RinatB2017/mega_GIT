#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = PelcoD_485

FOLDER      = RS232

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/rs232_5_mainbox \
    $$PWD/src/rs232_5_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

#DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

DEFINES += SAVE_WIDGETS_SPINBOX
DEFINES += SAVE_WIDGETS_SLIDER

HEADERS += \
    defines.hpp \
    version.hpp \
    rs232_5_mainbox.hpp

SOURCES += \
    rs232_5_mainbox.cpp \
    main.cpp

FORMS   += rs232_5_mainbox.ui

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)
include ($$LIB_PATH2/crc/crc.pri)

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
