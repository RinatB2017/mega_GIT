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

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    adc_oscilloscope_mainbox.hpp

SOURCES += \
    adc_oscilloscope_mainbox.cpp \
    main.cpp

FORMS   += adc_oscilloscope_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/meta/oscilloscopebox.pri)
include ($$LIB_PATH2/modbusasciiwidget/modbusasciiwidget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
