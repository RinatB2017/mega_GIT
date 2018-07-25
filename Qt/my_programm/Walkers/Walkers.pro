#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Walkers

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_youtube_mainbox \
    $$PWD/src/test_youtube_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column
QT  += webenginewidgets

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    test_youtube_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_youtube_mainbox.cpp \
    main.cpp

FORMS   += test_youtube_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include (src/youtube_walker/youtube_walker.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
