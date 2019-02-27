#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_OscilloscopeBox

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_OscilloscopeBox_mainbox \
    $$PWD/src/test_OscilloscopeBox_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += SAVE_WIDGETS_PROPERTIES
DEFINES += SAVE_WIDGETS_SLIDER

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    test_OscilloscopeBox_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_OscilloscopeBox_mainbox.cpp \
    main.cpp

FORMS   += test_OscilloscopeBox_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/oscilloscopebox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
