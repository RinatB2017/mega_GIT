#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
      $$PWD/src \
      $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += oscilloscope.hpp
SOURCES += oscilloscope.cpp
FORMS	+= ui/oscilloscope.ui
