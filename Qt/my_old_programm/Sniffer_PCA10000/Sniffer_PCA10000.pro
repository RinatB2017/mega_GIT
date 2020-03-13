#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Sniffer_PCA10000

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/sniffer_pca10000_mainbox \
    $$PWD/src/sniffer_pca10000_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT      += serialport

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    sniffer_pca10000_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    sniffer_pca10000_mainbox.cpp \
    main.cpp

FORMS   += sniffer_pca10000_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

include (src/test/test.pri)

include (src/PCA10000/pca10000.pri)

LIB_PATH = "$$PWD/../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/time/time.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
