#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui \
    $$PWD/test_data
INCLUDEPATH = $$DEPENDPATH

QT  += charts

HEADERS += candlestickdatareader.h
SOURCES += candlestickdatareader.cpp

HEADERS += candlestick_box.hpp
SOURCES += candlestick_box.cpp
FORMS   += candlestick_box.ui

RESOURCES   += candlestickdata.qrc
