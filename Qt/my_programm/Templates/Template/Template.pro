#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template

FOLDER  = _Templates

include (conf/conf.pri)

PROGRAMM_PATH  += \
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
#DEFINES += NO_LOG

#DEFINES += SAVE_INI

#DEFINES += ONLY_ENGLISH

#DEFINES += LOG_READ_ONLY

#DEFINES += NO_TOOLBAR_BUTTON_EXIT
#DEFINES += NO_TOOLBAR_BUTTON_FONT
#DEFINES += NO_TOOLBAR_BUTTON_LANG
#DEFINES += NO_TOOLBAR_BUTTON_STYLE
#DEFINES += NO_TOOLBAR_BUTTON_ABOUT
#DEFINES += NO_TOOLBAR_BUTTON_HELP
#DEFINES += NO_TOOLBAR_SEPARATORS

#DEFINES += SYSLOG_LOG

DEFINES += TOOLBAR_ORIENTATION

#DEFINES += DEMO
#DEFINES += "DEMO_YEAR=2018"
#DEFINES += "DEMO_MONTH=3"
#DEFINES += "DEMO_DAY=18"

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

#----------------------------------------------
LIB_PATH = "$$PWD/../../../lib"

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
    include ($$LIB_PATH/test_function/test_function.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/mymainwindow/mymainwindow.pri)
include (src/mainwidget/mainwidget.pri)
#----------------------------------------------

#message ($$DEFINES)

VPATH = $$INCLUDEPATH
