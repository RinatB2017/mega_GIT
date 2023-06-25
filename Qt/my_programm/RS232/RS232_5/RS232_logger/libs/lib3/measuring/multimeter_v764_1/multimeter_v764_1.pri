#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += v764_1_multimeter.hpp
SOURCES += v764_1_multimeter.cpp

HEADERS	+= v764_1_packets.hpp

HEADERS	+= v764_1_serialport_test.hpp
SOURCES	+= v764_1_serialport_test.cpp
