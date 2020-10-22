#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/macro
INCLUDEPATH = $$DEPENDPATH

QT      += serialport

HEADERS += serialterm.hpp
SOURCES += serialterm.cpp

HEADERS += const.h

SOURCES += macroThread.cpp
SOURCES += macroWorker.cpp
HEADERS += macroThread.h
HEADERS += macroWorker.h
