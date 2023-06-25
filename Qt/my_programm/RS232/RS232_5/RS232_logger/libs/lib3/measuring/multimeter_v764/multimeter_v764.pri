#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += v764_multimeter.hpp
SOURCES += v764_multimeter.cpp

HEADERS	+= v764_packets.hpp

HEADERS	+= v764_serialport_test.hpp
SOURCES	+= v764_serialport_test.cpp
