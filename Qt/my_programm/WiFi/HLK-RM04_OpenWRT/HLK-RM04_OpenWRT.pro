#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = HLK-RM04_OpenWRT

FOLDER  = WiFi

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/HLK-RM04_OpenWRT_mainbox \
    $$PWD/src/HLK-RM04_OpenWRT_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT      += serialport

HEADERS += \
    HLK-RM04_OpenWRT_mainbox.hpp \
    defines.hpp

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

VPATH = $$INCLUDEPATH
