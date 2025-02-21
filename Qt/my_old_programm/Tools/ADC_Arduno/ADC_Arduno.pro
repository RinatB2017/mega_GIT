#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = ADC_Arduno

FOLDER  = old_programm

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232LOG

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)

include (src/adc_arduino_mainbox/adc_arduino_mainbox.pri)

VPATH = $$INCLUDEPATH
