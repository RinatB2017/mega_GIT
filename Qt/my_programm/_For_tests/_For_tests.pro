#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = _For_tests

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE

#DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
#DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_MENU

#DEFINES += ONLY_ENGLISH

#DEFINES += NO_TOOLBAR_BUTTON_EXIT
#DEFINES += NO_TOOLBAR_BUTTON_FONT
#DEFINES += NO_TOOLBAR_BUTTON_LANG
#DEFINES += NO_TOOLBAR_BUTTON_STYLE
#DEFINES += NO_TOOLBAR_BUTTON_ABOUT
#DEFINES += NO_TOOLBAR_BUTTON_HELP
#DEFINES += NO_TOOLBAR_SEPARATORS

#DEFINES += DEMO
#DEFINES += "DEMO_YEAR=2018"
#DEFINES += "DEMO_MONTH=6"
#DEFINES += "DEMO_DAY=1"

#DEFINES += NO_LOG
#DEFINES += SYSLOG_LOG

#DEFINES += USE_TOPLEVELWIDGETS

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NEED_CODEC
DEFINES += SAVE_INI

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/puzzle/puzzle.qrc \
    images/images.qrc \
    ico/icons.qrc

#можно поставить новое logo
#RESOURCES += about/about.qrc

OTHER_FILES += \
    doc/notebook.txt
#    meta/*

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/icons/digits.pri)
include ($$LIB_PATH2/icons/arrows.pri)

include (src/for_tests_mainbox/for_tests_mainbox.pri)

include (src/mymainwindow/mymainwindow.pri)
include (src/simplewidget/simplewidget.pri)

include (src/supaplex/supaplex.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
