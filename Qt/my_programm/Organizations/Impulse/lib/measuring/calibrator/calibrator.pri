#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += H4_X_calibrator.hpp
SOURCES += H4_X_calibrator.cpp
    
HEADERS	+= H4_X_errors.hpp

HEADERS	+= H4_X_serialport_test.hpp
SOURCES	+= H4_X_serialport_test.cpp
