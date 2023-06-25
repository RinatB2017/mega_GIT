#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += oscilloscope_GDS_840C.hpp
SOURCES += oscilloscope_GDS_840C.cpp
FORMS   += oscilloscope_gds_840c.ui

HEADERS	+= fake_oscilloscope_GDS_840C.hpp
SOURCES	+= fake_oscilloscope_GDS_840C.cpp
