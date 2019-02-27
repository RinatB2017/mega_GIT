#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Walkers

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/walkers \
    $$PWD/src/walkers/ui
INCLUDEPATH = $$DEPENDPATH

QT  += webenginewidgets

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += SAVE_INI

DEFINES += SAVE_WIDGETS_TEXTEDIT

HEADERS += \
    walkers.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    walkers.cpp \
    main.cpp

FORMS   += walkers.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include (src/youtube_walker/youtube_walker.pri)
include (src/google_walker/google_walker.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
