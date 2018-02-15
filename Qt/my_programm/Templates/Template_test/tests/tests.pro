#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QT      += testlib widgets

HEADERS += test.hpp
SOURCES += test.cpp

LIB_PATH_TEST = "../../../../lib"
include ($$LIB_PATH_TEST/meta/mainwindow.pri)

include (../Template/src/mymainwindow/mymainwindow.pri)
include (../Template/src/mainwidget/mainwidget.pri)

VPATH = $$INCLUDEPATH
