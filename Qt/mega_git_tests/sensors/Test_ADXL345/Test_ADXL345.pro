#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_ADXL345

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
    arduino/ADXL345_simple/ADXL345_simple.ino

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/temperature.ico
}

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

include (src/test_ADXL345_mainbox/test_ADXL345_mainbox.pri)
#----------------------------------------------

VPATH = $$INCLUDEPATH
