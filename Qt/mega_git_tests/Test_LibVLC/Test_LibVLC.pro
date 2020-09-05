#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_LibVLC

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_LibVLC_mainbox \
    $$PWD/src/test_LibVLC_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_LibVLC_mainbox.hpp \
    defines.hpp \
    version.hpp \

SOURCES += \
    test_LibVLC_mainbox.cpp \
    main.cpp

FORMS   += test_LibVLC_mainbox.ui

OTHER_FILES += doc/notebook.txt

LIBS    += -lvlc

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "$$PWD/../../../Qt/lib"
include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
