#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_MODBUS

FOLDER  = modbus

DEPENDPATH  += \
    /usr/include/modbus \
    $$PWD/src \
    $$PWD/src/test_modbus_mainbox \
    $$PWD/src/test_modbus_mainbox/ui \
INCLUDEPATH = $$DEPENDPATH

QT      += serialport

DEFINES += NO_TESTBAR_GRAPHER
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_modbus_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_modbus_mainbox.cpp \
    main.cpp \

FORMS   += test_modbus_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

QT  += serialbus

LIB_PATH = "../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH

RESOURCES += \
    src/master.qrc
