#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Icons

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/icons_mainbox \
    $$PWD/src/icons_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR

#DEFINES += FIXED_SIZE

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    icons_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    icons_mainbox.cpp \
    main.cpp

FORMS   += icons_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

QMAKE_RESOURCE_FLAGS += -no-compress
include ($$LIB_PATH2/icons.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
