#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_OpenRPT

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_OpenRPT_mainbox \
    $$PWD/src/test_OpenRPT_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#DEFINES += FIXED_SIZE

DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_LOG

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_OpenRPT_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_OpenRPT_mainbox.cpp \
    main.cpp

FORMS   += test_OpenRPT_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/OpenRPT/OpenRPT.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
