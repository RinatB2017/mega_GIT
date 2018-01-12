#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Grapher

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/GrapherBox \
    $$PWD/src/GrapherBox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column


DEFINES += NO_TRAYICON

DEFINES += RS232
#DEFINES += RS232_LOG

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"

include ($$LIB_PATH/GrapherBox/GrapherBox.pri)

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/serial5/serial5.pri)
include ($$LIB_PATH/turbo.pri)
include ($$LIB_PATH/qwt/qwt.pri)
include ($$LIB_PATH/csvreader/csvreader.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
