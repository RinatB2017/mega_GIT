#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Emulator_MODBUS

FOLDER  = modbus

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT      += serialport network

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

#DEFINES += USE_HTONS
DEFINES += USE_CRC16
DEFINES += TEST

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

QT      += serialport
#HEADERS += settingsdialog.h
#SOURCES += settingsdialog.cpp
#FORMS   += settingsdialog.ui

win32 {
    RC_ICONS = ico/computer.ico
}

QT  += serialbus

LIB_PATH = "$$PWD/../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/emulator_modbus_mainbox/emulator_modbus_mainbox.pri)
include (src/settingsdialog/settingsdialog.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH

RESOURCES += \
    src/slave.qrc
