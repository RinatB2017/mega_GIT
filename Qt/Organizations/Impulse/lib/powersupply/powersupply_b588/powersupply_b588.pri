#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += b588_powersupply.hpp
SOURCES += b588_powersupply.cpp

HEADERS	+= b588_packets.hpp

HEADERS	+= b588_serialport_test.hpp
SOURCES	+= b588_serialport_test.cpp
