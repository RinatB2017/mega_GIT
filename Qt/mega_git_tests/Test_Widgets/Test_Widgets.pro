#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Widgets

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/test_Widgets_mainbox \
    $$PWD/src/test_Widgets_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE

#
#DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_MENU
#DEFINES += NO_LOG

#DEFINES += ONLY_ENGLISH

#DEFINES += LOG_READ_ONLY
#DEFINES += NEED_CORRECT_LOG

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_Widgets_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_Widgets_mainbox.cpp \
    main.cpp

FORMS   += test_Widgets_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include (src/primitives/hline/hline.pri)
include (src/primitives/vline/vline.pri)
include (src/primitives/widget/widget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
