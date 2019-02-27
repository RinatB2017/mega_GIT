#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_YouTube

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_youtube_mainbox \
    $$PWD/src/test_youtube_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

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
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "../../../Qt/lib"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
