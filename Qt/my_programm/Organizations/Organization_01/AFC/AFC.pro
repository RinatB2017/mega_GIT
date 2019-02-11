#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = AFC

FOLDER  = organizations/Organization_01

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/afc_mainbox \
    $$PWD/src/afc_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT      += serialport

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += SAVE_WIDGETS_PROPERTIES
DEFINES += SAVE_WIDGETS_DOUBLESPINBOX
DEFINES += SAVE_WIDGETS_SPINBOX
DEFINES += SAVE_WIDGETS_LINEEDIT
DEFINES += RS232_NO_FRAME

HEADERS += \
    defines.hpp \
    version.hpp \
    afc_mainbox.hpp

SOURCES += \
    afc_mainbox.cpp \
    main.cpp

FORMS   += afc_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH  = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH2/serial5/serial5.pri)
include ($$LIB_PATH2/crc/crc.pri)

include (../lib/measuring/multimeter_v786_2/multimeter_v786_2.pri)
include (../lib/measuring/calibrator/calibrator.pri)

include ($$LIB_PATH2/connection/connection.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
