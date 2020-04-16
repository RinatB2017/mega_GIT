#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = _For_tests

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/for_tests_mainbox \
    $$PWD/src/for_tests_mainbox/ui
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

DEFINES += SAVE_INI

DEFINES += PROGRAMM_IN_UTF8

DEFINES += NEED_CODEC

HEADERS += \
    for_tests_mainbox.hpp \
    defines.hpp \
    version.hpp

#DEFINES += XXX
#contains(DEFINES,XXX){
#    SOURCES += for_tests_mainbox.cpp \
#}

SOURCES += \
    for_tests_mainbox.cpp \
    main.cpp

FORMS   += for_tests_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc \
    images/bones/bones.qrc \
    images/gems/gems.qrc \
    images/puzzle/puzzle.qrc \
    ico/icons.qrc

#можно поставить новое logo
RESOURCES += about/about.qrc

OTHER_FILES += doc/notebook.txt

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/icons/digits.pri)

#include ($$LIB_PATH/mywidgets/mytimer.pri)

include (src/mymainwindow/mymainwindow.pri)
include (src/simple_mw/simple_mw.pri)

#include (src/test_widgets/widgets.pri)
include (src/simplewidget/simplewidget.pri)

contains(DEFINES,XXX){
    include (src/simplewidget/simplewidget.pri)
}

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH

#message ($$RESOURCES)

