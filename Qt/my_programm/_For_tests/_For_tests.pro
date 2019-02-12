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

QT          += widgets

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE
#
#DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
#DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_MENU

#DEFINES += ONLY_ENGLISH

#DEFINES += SAVE_WIDGETS_CHECKBOX
#DEFINES += SAVE_WIDGETS_COMBOBOX
#DEFINES += SAVE_WIDGETS_PUSHBUTTON
#DEFINES += SAVE_WIDGETS_TOOLBUTTON
#DEFINES += SAVE_WIDGETS_SPINBOX
#DEFINES += SAVE_WIDGETS_DOUBLESPINBOX
#DEFINES += SAVE_WIDGETS_SLIDER
#DEFINES += SAVE_WIDGETS_TEXTEDIT
#DEFINES += SAVE_WIDGETS_LINEEDIT

#DEFINES += NO_TOOLBAR_BUTTON_EXIT
#DEFINES += NO_TOOLBAR_BUTTON_FONT
#DEFINES += NO_TOOLBAR_BUTTON_LANG
#DEFINES += NO_TOOLBAR_BUTTON_STYLE
#DEFINES += NO_TOOLBAR_BUTTON_ABOUT
#DEFINES += NO_TOOLBAR_BUTTON_HELP
#DEFINES += NO_TOOLBAR_SEPARATORS

#DEFINES += NO_LOG_INFO
#DEFINES += NO_LOG_DEBUG
#DEFINES += NO_LOG_ERROR
#DEFINES += NO_LOG_TRACE

#DEFINES += DEMO
#DEFINES += "DEMO_YEAR=2018"
#DEFINES += "DEMO_MONTH=6"
#DEFINES += "DEMO_DAY=1"

#DEFINES += NO_LOG
#DEFINES += SYSLOG_LOG

#DEFINES += SAVE_INI

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    for_tests_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    for_tests_mainbox.cpp \
    main.cpp

FORMS   += for_tests_mainbox.ui

RESOURCES += ico/icons.qrc

win32 {
    RC_ICONS += ico/computer.ico
}

RESOURCES += \
    images/images.qrc

#можно поставить новое logo
RESOURCES += about/about.qrc

OTHER_FILES += doc/notebook.txt

LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/icons/digits.pri)

include (src/mymainwindow/mymainwindow.pri)
include ($$LIB_PATH2/secretbox/secretbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
