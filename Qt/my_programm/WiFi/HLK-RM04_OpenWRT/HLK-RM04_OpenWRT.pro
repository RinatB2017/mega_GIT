#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = HLK-RM04_OpenWRT

FOLDER  = WiFi

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/HLK-RM04_OpenWRT_mainbox \
    $$PWD/src/HLK-RM04_OpenWRT_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT      += serialport

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    HLK-RM04_OpenWRT_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    HLK-RM04_OpenWRT_mainbox.cpp \
    main.cpp

FORMS   += HLK-RM04_OpenWRT_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "$$PWD/../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
