#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Factory_Calibration

FOLDER  = organizations/Organization_01

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/reportstring \
    $$PWD/src/myreport
INCLUDEPATH = $$DEPENDPATH

QT      += multimedia
QT      += serialport

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += SAVE_WIDGETS_DOUBLESPINBOX
DEFINES += SAVE_WIDGETS_COMBOBOX
DEFINES += SAVE_WIDGETS_CHECKBOX
DEFINES += SAVE_WIDGETS_SPINBOX

DEFINES += PROGRAMM_IN_UTF8
DEFINES += FAST_COMMAND
DEFINES += NO_RESIZE

DEFINES += FAKE
#DEFINES += TEST

#DEFINES += USE_TEST_BOARD

#---
#CONFIG  += use_b588
CONFIG  += use_b590

#CONFIG  += use_v764
CONFIG  += use_v7642
#CONFIG  += use_v780

#CONFIG  += use_user_calibration
#---

use_user_calibration {
    DEFINES += USER_CALIBRATIONS
}

HEADERS += \
    defines.hpp \
    version.hpp \
    src/reportstring/reportstring.hpp \
    src/myreport/myreport.hpp

SOURCES += \
    main.cpp \
    src/reportstring/reportstring.cpp \
    src/myreport/myreport.cpp

OTHER_FILES += \
    doc/notebook.txt \
    report.xml

RESOURCES   += \
    pictures/pictures.qrc \
    music/music.qrc \
    report/report.qrc

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/time/time.pri)
include ($$LIB_PATH2/crc/crc.pri)

include ($$LIB_PATH2/database/database.pri)

include ($$LIB_PATH2/QHexEdit_QT5/QHexEdit_QT5.pri)

include ($$LIB_PATH2/OpenRPT/OpenRPT.pri)
include ($$LIB_PATH2/QtPropertyBrowser/qtpropertybrowser.pri)
include ($$LIB_PATH2/connection/connection.pri)

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

include (../lib/powersupply/parrotbar/parrotbar.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
