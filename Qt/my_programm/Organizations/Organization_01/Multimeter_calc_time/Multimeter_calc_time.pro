#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Multimeter_calc_time

FOLDER  = organizations/Organization_01

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/multimeter_calc_time_mainbox \
    $$PWD/src/multimeter_calc_time_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

QT      += serialport

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += SAVE_WIDGETS_SPINBOX

DEFINES += PROGRAMM_IN_UTF8

DEFINES += CONVERT_DATA_TO_ASCII
DEFINES += FAST_COMMAND


#DEFINES += USE_HTONS
DEFINES += FAKE

HEADERS += \
    defines.hpp \
    version.hpp \
    multimeter_calc_time_mainbox.hpp

SOURCES += \
    multimeter_calc_time_mainbox.cpp \
    main.cpp

FORMS   += multimeter_calc_time_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/crc/crc.pri)

include (../lib/measuring/multimeter_v786_2/multimeter_v786_2.pri)

include ($$LIB_PATH2/connection/connection.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
