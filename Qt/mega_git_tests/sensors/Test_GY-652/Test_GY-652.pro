#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_GY-652

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

win32 {
    DEFINES += NO_GRAPHER

    #DEFINES += NO_SHOW_TEMPERATURE
    #DEFINES += NO_SHOW_PRESSURE
    DEFINES += NO_SHOW_ATM
    DEFINES += NO_SHOW_ALTITUDE
}

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

OTHER_FILES += \
    doc/notebook.txt \
    arduino/Barometer_Sensor/Barometer_Sensor.ino

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

include (src/test_GY-652_mainbox/test_GY-652_mainbox.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    include ($$LIB_PATH/auto_inc_version.pri)
#}
#----------------------------------------------

VPATH = $$INCLUDEPATH
