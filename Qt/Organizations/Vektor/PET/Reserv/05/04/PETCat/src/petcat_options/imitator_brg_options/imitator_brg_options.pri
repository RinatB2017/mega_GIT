#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
      $$PWD/src \
      $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += imitator_brg_options.hpp
SOURCES += imitator_brg_options.cpp
FORMS	+= imitator_brg_options.ui

HEADERS += hexspinbox.hpp
SOURCES += hexspinbox.cpp
