#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT  *= charts

HEADERS += candlestickdatareader.h
SOURCES += candlestickdatareader.cpp

HEADERS += candlestick_box.hpp
SOURCES += candlestick_box.cpp
FORMS   += candlestick_box.ui

include ($$LIB_PATH2/icons/digits.pri)
include ($$LIB_PATH2/widgets/candlestick_box/myqchartview/myqchartview.pri)

RESOURCES   += $$PWD/test_data/candlestickdata.qrc
