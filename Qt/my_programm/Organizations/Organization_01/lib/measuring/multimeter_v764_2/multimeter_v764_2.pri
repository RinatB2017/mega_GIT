#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += v764_2_multimeter.hpp
SOURCES += v764_2_multimeter.cpp

HEADERS	+= v764_2_packets.hpp

HEADERS	+= v764_2_serialport_test.hpp
SOURCES	+= v764_2_serialport_test.cpp
