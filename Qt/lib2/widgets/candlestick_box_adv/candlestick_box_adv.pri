#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT      *= charts

HEADERS += candlestickdatareader.h
SOURCES += candlestickdatareader.cpp

HEADERS += candlestick_box_adv.hpp
SOURCES += candlestick_box_adv.cpp
FORMS   += candlestick_box_adv.ui

RESOURCES   += $$PWD/test_data/candlestickdata.qrc

include ($$LIB_PATH2/widgets/candlestick_box/myqchartview/myqchartview.pri)
include ($$LIB_PATH2/icons/digits.pri)
