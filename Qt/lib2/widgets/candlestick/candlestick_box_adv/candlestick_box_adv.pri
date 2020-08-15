#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT      *= charts

HEADERS += candlestick_box_adv.hpp
SOURCES += candlestick_box_adv.cpp
FORMS   += candlestick_box_adv.ui
