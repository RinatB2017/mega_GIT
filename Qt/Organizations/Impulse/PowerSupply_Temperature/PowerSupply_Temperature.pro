#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = PowerSupply_Temperature

FOLDER  = organizations/Impulse

DEPENDPATH  += \
    $$PWD \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

QT      += serialport

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR

DEFINES += NO_TRAYICON

DEFINES += USE_SCALE_POINT_TIME
#DEFINES += NO_GRAPH

DEFINES += SAVE_WIDGETS_CHECKBOX
DEFINES += SAVE_WIDGETS_COMBOBOX
DEFINES += SAVE_WIDGETS_SPINBOX

DEFINES += FAST_COMMAND
#DEFINES += FAKE
DEFINES += ADD_TOOLBAR

#CONFIG  += use_b588
CONFIG  += use_b590

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/power_knock/power_knock.pri)
include ($$LIB_PATH/meta/grapherbox.pri)
include ($$LIB_PATH/crc/crc.pri)
include ($$LIB_PATH/connection/connection.pri)
include ($$LIB_PATH/QHexEdit_QT5/QHexEdit_QT5.pri)

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
