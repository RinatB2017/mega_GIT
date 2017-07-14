#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
      $$PWD/src \
      $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += page_imitator_brg_options.hpp
SOURCES += page_imitator_brg_options.cpp
FORMS	+= page_imitator_brg_options.ui

HEADERS += hexspinbox.hpp
SOURCES += hexspinbox.cpp
