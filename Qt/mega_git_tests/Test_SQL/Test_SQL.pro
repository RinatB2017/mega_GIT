#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_SQL

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_SQL_mainbox \
    $$PWD/src/test_SQL_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += sql

DEFINES += FIXED_SIZE

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    test_SQL_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_SQL_mainbox.cpp \
    main.cpp

FORMS   += test_SQL_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/OpenRPT/OpenRPT.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH

