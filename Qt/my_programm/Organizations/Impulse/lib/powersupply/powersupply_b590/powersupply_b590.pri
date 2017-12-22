#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += b590_powersupply.hpp
SOURCES += b590_powersupply.cpp

HEADERS	+= b590_defines.hpp
HEADERS += b590_packets.hpp

HEADERS	+= b590_serialport_test.hpp
SOURCES	+= b590_serialport_test.cpp
