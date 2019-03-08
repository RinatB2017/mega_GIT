#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_md5

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_md5_mainbox \
    $$PWD/src/test_md5_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_md5_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_md5_mainbox.cpp \
    main.cpp

FORMS   += test_md5_mainbox.ui

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
