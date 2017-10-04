#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = RS485

FOLDER      = RS485

DEPENDPATH  += \
    ../common \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

DEFINES += DEMO
DEFINES += "DEMO_YEAR=2018"
DEFINES += "DEMO_MONTH=1"
DEFINES += "DEMO_DAY=1"

DEFINES += SAVE_WIDGETS_SPINBOX

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp \
    packet.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS += mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../lib"
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/serial5/serial5.pri)
include ($$LIB_PATH/widgets/hexspinbox/hexspinbox.pri)
include ($$LIB_PATH/crc/crc.pri)

include (src/pelcod/pelcod.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

lessThan(QT_MAJOR_VERSION, 5) {
    error (Only Qt5)
}

VPATH = $$INCLUDEPATH
