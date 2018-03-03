#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = CalcMd5

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/calcmd5_mainbox \
    $$PWD/src/calcmd5_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_LOG
DEFINES += NO_MENU
DEFINES += NO_TOOLBAR
DEFINES += NO_STATUSBAR
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += FIXED_SIZE

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    calcmd5_mainbox.hpp

SOURCES += \
    calcmd5_mainbox.cpp \
    main.cpp

FORMS   += calcmd5_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/turbo.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
