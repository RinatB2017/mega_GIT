#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = File_to_RS232

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/file_to_rs232_mainbox \
    $$PWD/src/file_to_rs232_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    file_to_rs232_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    file_to_rs232_mainbox.cpp \
    main.cpp

FORMS   += file_to_rs232_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)
include ($$LIB_PATH2/qwt/qwt.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
