#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_LibVLC

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/Test_LibVLC_mainbox \
    $$PWD/src/Test_LibVLC_mainbox/ui
INCLUDEPATH = $$DEPENDPATH



DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    Test_LibVLC_mainbox.hpp

SOURCES += \
    Test_LibVLC_mainbox.cpp \
    main.cpp

FORMS   += Test_LibVLC_mainbox.ui

OTHER_FILES += doc/notebook.txt

LIBS    += -lvlc

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "../../../Qt/lib"
include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
