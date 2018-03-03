#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Trader

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/trader \
    $$PWD/src/Test_Trader_mainbox \
    $$PWD/src/Test_Trader_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += FIXED_SIZE

#
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_LOG

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
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
    Test_Trader_mainbox.hpp

SOURCES += \
    traderplot.cpp \
    Test_Trader_mainbox.cpp \
    main.cpp

FORMS   += Test_Trader_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
