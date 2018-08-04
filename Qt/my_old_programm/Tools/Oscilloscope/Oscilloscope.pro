#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Oscilloscope

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/oscilloscope_mainbox \
    $$PWD/src/oscilloscope_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    oscilloscope_mainbox.hpp

SOURCES += \
    oscilloscope_mainbox.cpp \
    main.cpp

FORMS   += oscilloscope_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH2/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH2/qwt/qwt.pri)
include ($$LIB_PATH/turbo.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
