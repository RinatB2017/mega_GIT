#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_r4_serial_widget

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/serial \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += FLAG_RESIZE
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp \
    delegates.h \
    angle_line_edit.h \
    worker.hpp

SOURCES += \
    mainbox.cpp \
    worker.cpp \
    main.cpp

FORMS += mainbox.ui

LIB_PATH = "../../../../lib"

include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)

VERSION_HEADER = src/version.hpp
include ($$LIB_PATH/auto_inc_version.pri)

include (serial/r4_serial.pri)

win32 {
    error ("In windows is not working")
}

VPATH = $$INCLUDEPATH
