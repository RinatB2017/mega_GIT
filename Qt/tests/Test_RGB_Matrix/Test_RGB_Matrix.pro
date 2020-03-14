#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Test_RGB_Matrix

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/diod \
    $$PWD/src/display \
    $$PWD/src/palette \
    $$PWD/src/test_RGB_Matrix_mainbox \
    $$PWD/src/test_RGB_Matrix_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

DEFINES += MODBUS

HEADERS += \
    test_RGB_Matrix_mainbox.hpp \
    defines.hpp \
    version.hpp \
    palette.hpp

SOURCES += \
    test_RGB_Matrix_mainbox.cpp \
    palette.cpp \
    main.cpp \

FORMS   += test_RGB_Matrix_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/time/time.pri)

include ($$LIB_PATH2/crc/crc.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serial5.pri)
include ($$LIB_PATH2/display/display.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
