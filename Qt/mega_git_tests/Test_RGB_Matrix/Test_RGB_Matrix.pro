#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Test_RGB_Matrix

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/diod
INCLUDEPATH = $$DEPENDPATH

DEFINES += MODBUS

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp \

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
include ($$LIB_PATH2/palette/palette.pri)

include (src/test_RGB_Matrix_mainbox/test_RGB_Matrix_mainbox.pri)

VPATH = $$INCLUDEPATH
