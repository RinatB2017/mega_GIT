#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_ADC

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_ADC_mainbox \
    $$PWD/src/test_ADC_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#DEFINES += USE_SCALE_POINT_DATETIME

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_ADC_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_ADC_mainbox.cpp \
    main.cpp

FORMS   += test_ADC_mainbox.ui

OTHER_FILES += \
    doc/notebook.txt \
    arduino/arduino.ino

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/I2c_logo.ico
}

RESOURCES += \
    ico/icons.qrc

#----------------------------------------------
LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)

#include ($$LIB_PATH2/serial5/serial5.pri)
include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate_win7/serialbox5_fix_baudrate_win7.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
