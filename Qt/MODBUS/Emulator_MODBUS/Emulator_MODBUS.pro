#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Emulator_MODBUS

FOLDER  = modbus

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui \
    $$PWD/src/settingsdialog \
    $$PWD/src/settingsdialog/ui \
INCLUDEPATH = $$DEPENDPATH

QT      += serialport network
QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += FLAG_RESIZE
DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

#DEFINES += USE_HTONS
DEFINES += USE_CRC16
DEFINES += TEST

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS   += mainbox.ui

QT      += serialport
HEADERS += settingsdialog.h
SOURCES += settingsdialog.cpp
FORMS   += settingsdialog.ui

win32 {
    RC_FILE = ico/myapp.rc
}

QT  += serialbus

LIB_PATH = "../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH

RESOURCES += \
    src/slave.qrc
