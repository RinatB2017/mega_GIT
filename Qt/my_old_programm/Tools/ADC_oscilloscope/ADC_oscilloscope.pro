#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = ADC_oscilloscope

FOLDER  = old_programm

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

QT  += serialport

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/meta/oscilloscopebox.pri)
include ($$LIB_PATH2/modbusasciiwidget/modbusasciiwidget.pri)

include (src/adc_oscilloscope_mainbox/adc_oscilloscope_mainbox.pri)

VPATH = $$INCLUDEPATH
