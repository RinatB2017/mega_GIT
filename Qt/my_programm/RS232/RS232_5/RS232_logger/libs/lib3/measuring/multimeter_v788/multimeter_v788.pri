#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += v788_multimeter.hpp
SOURCES += v788_multimeter.cpp

HEADERS	+= v788_packets.hpp

HEADERS	+= v788_serialport_test.hpp
SOURCES	+= v788_serialport_test.cpp
