#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Multimeter_V7XX

FOLDER  = organizations/Impulse

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += USE_SCALE_POINT_DATETIME
DEFINES += USE_SCALE_POINT_TIME

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

DEFINES += SAVE_WIDGETS_DOUBLESPINBOX
DEFINES += SAVE_WIDGETS_SPINBOX

DEFINES += MEASURING_CURRENT_IN_SHUNT


DEFINES += FAST_COMMAND
#DEFINES += FAKE

QT      += serialport

#CONFIG  += use_v764
CONFIG  += use_v7642
#CONFIG  += use_v780

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS   += mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include ($$LIB_PATH/power_knock/power_knock.pri)
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/time/time.pri)
include ($$LIB_PATH/crc/crc.pri)

include ($$LIB_PATH/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH/csvreader/csvreader.pri)
include ($$LIB_PATH/qwt/qwt.pri)

use_v764 {
    DEFINES += V764
    include (../lib/measuring/multimeter_v764/multimeter_v764.pri)
}

use_v7642 {
    DEFINES += V7642
    include (../lib/measuring/multimeter_v764_2/multimeter_v764_2.pri)
}

use_v780 {
    DEFINES += V780
    include (../lib/measuring/multimeter_v780/multimeter_v780.pri)
}

include ($$LIB_PATH2/connection/connection.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
