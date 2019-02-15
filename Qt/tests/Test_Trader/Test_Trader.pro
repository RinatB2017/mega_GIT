#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Trader

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/trader \
    $$PWD/src/test_trader_mainbox \
    $$PWD/src/test_trader_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_LOG

DEFINES += PROGRAMM_IN_UTF8

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
    version.hpp \
    test_trader_mainbox.hpp

SOURCES += \
    traderplot.cpp \
    test_trader_mainbox.cpp \
    main.cpp

FORMS   += test_trader_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH2/qwt/qwt.pri)

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
