#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/serial/terminal
INCLUDEPATH = $$DEPENDPATH

HEADERS += terminal.hpp
SOURCES += terminal.cpp

include (serial/serial.pri)
