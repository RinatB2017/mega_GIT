#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_PowerSupply

FOLDER  = organizations/Impulse

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

QT      += serialport

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += SAVE_WIDGETS_CHECKBOX
DEFINES += SAVE_WIDGETS_COMBOBOX
DEFINES += SAVE_WIDGETS_DOUBLESPINBOX
DEFINES += SAVE_WIDGETS_SPINBOX

DEFINES += NO_TRAYICON
DEFINES += NO_TEST

DEFINES += CONVERT_DATA_TO_ASCII
#DEFINES += USE_HTONS

DEFINES += FAST_COMMAND
#DEFINES += FAKE
DEFINES += ADD_TOOLBAR
#DEFINES += DISABLE_CORRIDOR

#CONFIG  += use_b588
CONFIG  += use_b590

CONFIG  += use_v764
#CONFIG  += use_v7642

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include ($$LIB_PATH2/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH/time/time.pri)
include ($$LIB_PATH2/qwt/qwt.pri)
include ($$LIB_PATH2/crc/crc.pri)

include (../lib/measuring/multimeter_v786_2/multimeter_v786_2.pri)

use_v764 {
    DEFINES += USE_V764
    include (../lib/measuring/multimeter_v764/multimeter_v764.pri)
}
use_v7642 {
    DEFINES += USE_V7642
    include (../lib/measuring/multimeter_v764_2/multimeter_v764_2.pri)
}

include ($$LIB_PATH2/connection/connection.pri)

include ($$LIB_PATH2/QHexEdit_QT5/QHexEdit_QT5.pri)

use_b588 {
    message (use B5-88)
    DEFINES += USE_B588
    include (../lib/powersupply/powersupply_b588/powersupply_b588.pri)
    include (../lib/powersupply/toolbar_b588/toolbar_b588.pri)
    include (../lib/powersupply/fram_b588/fram_b588.pri)
    include (src/B588/b588.pri)
}
use_b590 {
    message (use B5-90)
    DEFINES += USE_B590
    include (../lib/powersupply/powersupply_b590/powersupply_b590.pri)
    include (../lib/powersupply/toolbar_b590/toolbar_b590.pri)
    include (../lib/powersupply/fram_b590/fram_b590.pri)
    include (src/B590/b590.pri)

    include (../lib/powersupply/find_powersupply/find_powersupply.pri)
}

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
