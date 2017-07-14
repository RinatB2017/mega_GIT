#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

LIBS += -lfftw3 -lm

HEADERS += qfreq.h
SOURCES += qfreq.cpp

RESOURCES += qfreq.qrc
