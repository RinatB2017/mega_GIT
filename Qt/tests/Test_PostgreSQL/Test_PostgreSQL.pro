#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_PostgreSQL

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/Test_PostgreSQL_mainbox \
    $$PWD/src/Test_PostgreSQL_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += sql

DEFINES += NO_TOOLBAR

DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    Test_PostgreSQL_mainbox.hpp

SOURCES += \
    Test_PostgreSQL_mainbox.cpp \
    main.cpp

FORMS   += Test_PostgreSQL_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "../../../Qt/lib"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/turbo.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
