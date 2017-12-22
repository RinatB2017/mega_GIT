#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
      $$PWD/src \
      $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += oscilloscope_reader.hpp
SOURCES += oscilloscope_reader.cpp
FORMS	+= ui/oscilloscope_reader.ui
