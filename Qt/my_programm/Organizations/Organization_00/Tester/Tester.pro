#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Tester

FOLDER  = organizations/Organization_00

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    tester.hpp

SOURCES += \
    tester.cpp \
    main.cpp

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../../lib"

#include ($$LIB_PATH/turbo.pri)
#include (resources.pri)

#VERSION_HEADER = src/version.hpp
#include ($$LIB_PATH/auto_inc_version.pri)

QMAKE_CFLAGS    += -std=c++11
QMAKE_CXXFLAGS  += -std=c++11
QMAKE_LFLAGS    += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4) {
    message(Qt5)
    QT += widgets
}

VPATH = $$INCLUDEPATH
