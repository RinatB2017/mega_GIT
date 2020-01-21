#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_PostgreSQL

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_PostgreSQL_mainbox \
    $$PWD/src/test_PostgreSQL_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += sql

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_PostgreSQL_mainbox.hpp \
    defines.hpp \
    version.hpp \

SOURCES += \
    test_PostgreSQL_mainbox.cpp \
    main.cpp

FORMS   += test_PostgreSQL_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
