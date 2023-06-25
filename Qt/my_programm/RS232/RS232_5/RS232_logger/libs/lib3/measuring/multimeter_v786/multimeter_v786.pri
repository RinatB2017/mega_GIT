#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += v786_multimeter.hpp
SOURCES += v786_multimeter.cpp

HEADERS	+= v786_union.hpp
HEADERS	+= v786_packets.hpp

HEADERS	+= v786_serialport_test.hpp
SOURCES	+= v786_serialport_test.cpp
