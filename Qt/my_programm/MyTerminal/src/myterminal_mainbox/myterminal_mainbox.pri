#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    /usr/include/qtermwidget5 \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += myterminal_mainbox.hpp
SOURCES += myterminal_mainbox.cpp
FORMS   += myterminal_mainbox.ui
