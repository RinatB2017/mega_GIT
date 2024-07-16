#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Trader

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/trader \
    $$PWD/src/test_trader_mainbox \
    $$PWD/src/test_trader_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    curvedata.hpp \
    plot.h \
    quotefactory.h \
    griditem.h \
    legend.h

SOURCES += \
    griditem.cpp \
    legend.cpp \
    quotefactory.cpp \
    plot.cpp

HEADERS += \
    traderplot.hpp \
    defines.hpp \
    test_trader_mainbox.hpp

SOURCES += \
    traderplot.cpp \
    test_trader_mainbox.cpp \
    main.cpp

FORMS   += test_trader_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH2/qwt/qwt.pri)

win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = c:/Programming/my_programm_bin/debug
    }
    else {
        DESTDIR = c:/Programming/my_programm_bin/release
    }
}

VPATH = $$INCLUDEPATH
