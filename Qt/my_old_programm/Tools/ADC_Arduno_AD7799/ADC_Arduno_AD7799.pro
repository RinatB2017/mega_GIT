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

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += RS232LOG
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    adc_arduino_ad7799_mainbox.hpp

SOURCES += \
    adc_arduino_ad7799_mainbox.cpp \
    main.cpp

FORMS   += adc_arduino_ad7799_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH2/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH2/serial5/serial5.pri)
include ($$LIB_PATH/turbo.pri)
include ($$LIB_PATH2/qwt/qwt.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
