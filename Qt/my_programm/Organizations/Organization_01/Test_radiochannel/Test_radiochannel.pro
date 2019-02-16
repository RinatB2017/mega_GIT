#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_radiochannel

FOLDER  = organizations/Organization_01

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_radiochannel_mainbox \
    $$PWD/src/test_radiochannel_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT      += serialport

DEFINES += USE_SCALE_POINT_TIME

DEFINES += SAVE_WIDGETS_PROPERTIES
DEFINES += SAVE_WIDGETS_SPINBOX

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    test_radiochannel_mainbox.hpp \
    defines.hpp \
    version.hpp \

SOURCES += \
    test_radiochannel_mainbox.cpp \
    main.cpp

FORMS   += test_radiochannel_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include ($$LIB_PATH2/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH/time/time.pri)
include ($$LIB_PATH2/qwt/qwt.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
