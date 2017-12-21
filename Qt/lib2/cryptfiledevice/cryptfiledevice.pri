#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QMAKE_CFLAGS   += -std=c++11
QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS   += -std=c++11
LIBS += -lcrypto

HEADERS += cryptfiledevice.h
SOURCES += cryptfiledevice.cpp
