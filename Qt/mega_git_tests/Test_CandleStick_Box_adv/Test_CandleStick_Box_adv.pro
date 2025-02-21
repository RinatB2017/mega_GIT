#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_CandleStick_Box_adv

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH2/widgets/candlestick/candlestick.pri)

include (src/test_candlestick_box_mainbox/test_candlestick_box_mainbox.pri)

# https://forum.qt.io/topic/69626/visualize-value-on-qt-chart-view/3
# mapToValue

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = c:/Programming/my_programm_bin/debug
    }
    else {
        DESTDIR = c:/Programming/my_programm_bin/release
    }
}

VPATH = $$INCLUDEPATH
