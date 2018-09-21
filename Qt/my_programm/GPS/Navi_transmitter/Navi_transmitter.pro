#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Navi_transmitter

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/navi_transmitter_mainbox \
    $$PWD/src/navi_transmitter_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += SAVE_WIDGETS_SPINBOX

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON

#DEFINES += NO_LOG

HEADERS += \
    navi_transmitter_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    navi_transmitter_mainbox.cpp \
    main.cpp

FORMS   += navi_transmitter_mainbox.ui

RESOURCES += doc/test_data.qrc

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH  = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
