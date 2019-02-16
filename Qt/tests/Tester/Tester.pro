#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Tester

FOLDER  = tests

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH



DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    tester.hpp

SOURCES += \
    tester.cpp \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "../../../Qt/lib"

#include ($$LIB_PATH/turbo.pri)
#include (resources.pri)

#VERSION_HEADER = src/version.hpp
#include ($$LIB_PATH/auto_inc_version.pri)

greaterThan(QT_MAJOR_VERSION, 4) {
    message(Qt5)
    QT += widgets
}

VPATH = $$INCLUDEPATH
