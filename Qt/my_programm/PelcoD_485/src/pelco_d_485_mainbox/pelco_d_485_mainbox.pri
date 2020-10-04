#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += pelco_d_485_mainbox.hpp
SOURCES += pelco_d_485_mainbox.cpp
FORMS   += pelco_d_485_mainbox.ui
