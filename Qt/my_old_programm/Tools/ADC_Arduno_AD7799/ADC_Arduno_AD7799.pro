#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = ADC_Arduno_AD7799

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/adc_arduino_ad7799_mainbox \
    $$PWD/src/adc_arduino_ad7799_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232LOG
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    adc_arduino_ad7799_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    adc_arduino_ad7799_mainbox.cpp \
    main.cpp

FORMS   += adc_arduino_ad7799_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH2/serial5/serialbox5/serialbox5.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
