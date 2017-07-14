#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
      $$PWD/src \
      $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += oscilloscope_writer.hpp
SOURCES += oscilloscope_writer.cpp
FORMS	+= oscilloscope_writer.ui
