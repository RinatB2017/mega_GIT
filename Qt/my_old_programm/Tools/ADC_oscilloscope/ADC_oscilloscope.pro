#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = ADC_oscilloscope

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/adc_oscilloscope_mainbox \
    $$PWD/src/adc_oscilloscope_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += serialport

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    adc_oscilloscope_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    adc_oscilloscope_mainbox.cpp \
    main.cpp

FORMS   += adc_oscilloscope_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/meta/oscilloscopebox.pri)
include ($$LIB_PATH2/modbusasciiwidget/modbusasciiwidget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
