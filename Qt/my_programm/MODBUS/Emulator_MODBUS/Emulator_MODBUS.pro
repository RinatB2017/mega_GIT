#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Emulator_MODBUS

FOLDER  = modbus

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT      += serialport network

#DEFINES += USE_HTONS
DEFINES += USE_CRC16
DEFINES += TEST

HEADERS += \
    defines.hpp

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

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/emulator_modbus_mainbox/emulator_modbus_mainbox.pri)
include (src/settingsdialog/settingsdialog.pri)

VPATH = $$INCLUDEPATH

RESOURCES += \
    src/slave.qrc
