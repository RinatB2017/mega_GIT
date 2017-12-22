#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += v786_2_multimeter.hpp
SOURCES += v786_2_multimeter.cpp

HEADERS	+= v786_2_union.hpp
HEADERS	+= v786_2_packets.hpp

HEADERS	+= v786_2_serialport_test.hpp
SOURCES	+= v786_2_serialport_test.cpp