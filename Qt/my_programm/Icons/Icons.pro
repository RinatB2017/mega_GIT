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

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR

#DEFINES += FIXED_SIZE

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    icons_mainbox.hpp

SOURCES += \
    icons_mainbox.cpp \
    main.cpp

FORMS   += icons_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/mywidgets/myfiledialog.pri)

QMAKE_RESOURCE_FLAGS += -no-compress
include ($$LIB_PATH2/icons.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
