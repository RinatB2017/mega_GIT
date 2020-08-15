#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_CandleStick_Box

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_LOG

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \

SOURCES += \
    main.cpp

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH2/widgets/candlestick/candlestick.pri)

include (src/test_candlestick_box_mainbox/test_candlestick_box_mainbox.pri)

# https://forum.qt.io/topic/69626/visualize-value-on-qt-chart-view/3
# mapToValue

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = c:/Programming/my_programm_bin/debug
    }
    else {
        DESTDIR = c:/Programming/my_programm_bin/release
    }
}

VPATH = $$INCLUDEPATH
