#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_MPU-6050

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

OTHER_FILES += \
    doc/notebook.txt \
    arduino/MPU-6050/MPU-6050.ino \
    example/scepter/scepter.ino \
    example/gy_521_send_serial/gy_521_send_serial.ino \
    example/ShowGY521Data/ShowGY521Data.pde \
    example/43/43.c \
    example/43/bcd.c \
    example/43/bcd.h \
    example/43/compilers_4.h \
    example/43/i2c.h \
    example/43/lcd_lib_2.c \
    example/43/lcd_lib_2.h \
    example/43/port_macros.h

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/temperature.ico
}

include (src/glwidget/glwidget_qt5.pri)
include (src/test_glwidget/test_glwidget.pri)
include (src/lcd_widget/lcd_widget.pri)

RESOURCES += \
    ico/icons.qrc

#----------------------------------------------
LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
#include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate_win7/serialbox5_fix_baudrate_win7.pri)
include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate/serialbox5_fix_baudrate.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)

include (src/test_MPU-6050_mainbox/test_MPU-6050_mainbox.pri)
#----------------------------------------------

VPATH = $$INCLUDEPATH
