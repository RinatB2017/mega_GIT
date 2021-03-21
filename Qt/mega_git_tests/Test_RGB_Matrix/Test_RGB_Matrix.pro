#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Test_RGB_Matrix

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/diod \
    $$PWD/src/display \
    $$PWD/src/palette \
    $$PWD/src/test_RGB_Matrix_mainbox \
    $$PWD/src/test_RGB_Matrix_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += MODBUS

HEADERS += \
    test_RGB_Matrix_mainbox.hpp \
    defines.hpp \
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

VPATH = $$INCLUDEPATH
