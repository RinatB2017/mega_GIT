#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += H4_X_calibrator.hpp
SOURCES += H4_X_calibrator.cpp
    
HEADERS	+= H4_X_errors.hpp
HEADERS += H4_X_keys.hpp

HEADERS	+= H4_X_serialport_test.hpp
SOURCES	+= H4_X_serialport_test.cpp

HEADERS	+= H4_X_serialport_test_param.hpp
SOURCES	+= H4_X_serialport_test_param.cpp

FORMS	+= H4_X_serialport_test_param.ui
