#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = MTC

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mtc_mainbox \
    $$PWD/src/mtc_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += RS232_FIXED_SIZE

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    mtc_mainbox.hpp

SOURCES += \
    mtc_mainbox.cpp \
    main.cpp

FORMS   += mtc_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
