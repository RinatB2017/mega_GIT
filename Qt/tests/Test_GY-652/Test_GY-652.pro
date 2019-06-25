#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_GY-652

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_GY-652_mainbox \
    $$PWD/src/test_GY-652_mainbox/ui \
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_TOOLBAR
DEFINES += SHOW_SIZE
DEFINES += NO_MENU

DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_LOG

win32 {
    DEFINES += NO_GRAPHER

    #DEFINES += NO_SHOW_TEMPERATURE
    #DEFINES += NO_SHOW_PRESSURE
    DEFINES += NO_SHOW_ATM
    DEFINES += NO_SHOW_ALTITUDE
}

HEADERS += \
    test_GY-652_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_GY-652_mainbox.cpp \
    main.cpp

FORMS   += test_GY-652_mainbox.ui

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
LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
