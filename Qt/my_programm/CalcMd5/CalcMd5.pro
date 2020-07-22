#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = CalcMd5

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

#DEFINES += NO_LOG
#DEFINES += NO_MENU
#DEFINES += NO_TOOLBAR
#DEFINES += NO_STATUSBAR
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

#DEFINES += FIXED_SIZE

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/calcmd5_mainbox/calcmd5_mainbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
