#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT      *= charts

HEADERS += candlestick_box.hpp
SOURCES += candlestick_box.cpp
FORMS   += candlestick_box.ui
