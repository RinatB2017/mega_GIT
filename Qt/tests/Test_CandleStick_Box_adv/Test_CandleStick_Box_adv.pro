#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_CandleStick_Box_adv

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/trader \
    $$PWD/src/test_candlestick_box_mainbox \
    $$PWD/src/test_candlestick_box_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_LOG

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_candlestick_box_mainbox.hpp \
    defines.hpp \
    version.hpp \

SOURCES += \
    test_candlestick_box_mainbox.cpp \
    main.cpp

FORMS   += test_candlestick_box_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH2/widgets/candlestick_box_adv/candlestick_box_adv.pri)

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
