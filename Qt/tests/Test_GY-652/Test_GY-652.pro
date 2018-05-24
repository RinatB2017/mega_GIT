#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_GY-652

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/Test_GY-652_mainbox \
    $$PWD/src/Test_GY-652_mainbox/ui \
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_TOOLBAR
DEFINES += NO_MENU
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_GRAPHER
DEFINES += NO_LOG

HEADERS += \
    defines.hpp \
    version.hpp \
    Test_GY-652_mainbox.hpp

SOURCES += \
    Test_GY-652_mainbox.cpp \
    main.cpp

FORMS   += Test_GY-652_mainbox.ui

OTHER_FILES += \
    doc/notebook.txt \
    arduino/Barometer_Sensor/Barometer_Sensor.ino

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_FILE = ico/myapp.rc
}

#----------------------------------------------
LIB_PATH = "../../../Qt/lib"
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
