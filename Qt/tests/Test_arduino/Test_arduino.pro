#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_arduino

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_arduino_mainbox \
    $$PWD/src/test_arduino_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

DEFINES += SAVE_WIDGETS_PROPERTIES
DEFINES += SAVE_WIDGETS_SLIDER

HEADERS += \
    test_arduino_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_arduino_mainbox.cpp \
    main.cpp

FORMS   += test_arduino_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
    LIBS += -lws2_32
}

LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
