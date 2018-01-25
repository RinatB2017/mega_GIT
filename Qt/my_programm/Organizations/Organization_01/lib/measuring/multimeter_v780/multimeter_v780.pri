#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += v780_multimeter.hpp
SOURCES += v780_multimeter.cpp

HEADERS	+= v780_packets.hpp

HEADERS	+= v780_serialport_test.hpp
SOURCES	+= v780_serialport_test.cpp
