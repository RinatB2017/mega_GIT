#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += datagrapherbox.hpp
SOURCES += datagrapherbox.cpp
FORMS   += datagrapherbox.ui

include (adc_label/adc_label.pri)
