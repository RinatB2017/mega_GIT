#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Walkers

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/walkers \
    $$PWD/src/walkers/ui
INCLUDEPATH = $$DEPENDPATH

QT  += webenginewidgets

DEFINES += SAVE_INI

HEADERS += \
    walkers.hpp \
    defines.hpp

SOURCES += \
    walkers.cpp \
    main.cpp

FORMS   += walkers.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "$$PWD/../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include (src/youtube_walker/youtube_walker.pri)
include (src/google_walker/google_walker.pri)

VPATH = $$INCLUDEPATH
