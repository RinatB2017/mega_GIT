#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Multimeter_V786

FOLDER  = organizations/Impulse

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/multimeterbox/ \
    $$PWD/src/multimeterbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON
#DEFINES += TESTBAR

DEFINES += USE_SCALE_POINT_DATETIME
#DEFINES += USE_SCALE_POINT_TIME

DEFINES += SAVE_WIDGETS_SPINBOX
#DEFINES += SAVE_WIDGETS_LINEEDIT

DEFINES += CONVERT_DATA_TO_ASCII

#DEFINES += FAST_COMMAND
DEFINES += ADD_MENU

#DEFINES += USE_HTONS
#DEFINES += FAKE

QT      += serialport

HEADERS += \
    src/multimeterbox/multimeterbox.hpp \
    src/defines.hpp \
    src/version.hpp

SOURCES += \
    src/multimeterbox/multimeterbox.cpp \
    src/main.cpp

#DEFINES += NO_GRAPHER
#FORMS   += src/multimeterbox/ui/multimeterbox_no_grapher.ui
FORMS   += src/multimeterbox/ui/multimeterbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include (../common/batteryprogressbar/batteryprogressbar.pri)
include (../common/handcontrol/handcontrol.pri)

include ($$LIB_PATH/power_knock/power_knock.pri)
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH/csvreader/csvreader.pri)
include ($$LIB_PATH/qwt/qwt.pri)
include ($$LIB_PATH/crc/crc.pri)

include (../lib/measuring/multimeter_v786_2/multimeter_v786_2.pri)

include ($$LIB_PATH2/connection/connection.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH

