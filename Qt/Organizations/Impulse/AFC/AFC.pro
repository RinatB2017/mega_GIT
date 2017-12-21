#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = AFC

FOLDER  = organizations/Impulse

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

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
    mainbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS += mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/meta/grapherbox.pri)
include ($$LIB_PATH/serial5/serial5.pri)
include ($$LIB_PATH/crc/crc.pri)

include (../lib/measuring/multimeter_v786_2/multimeter_v786_2.pri)
include (../lib/measuring/calibrator/calibrator.pri)

include ($$LIB_PATH2/connection/connection.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
