#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/grapherbox \
    $$PWD/src/grapherbox/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += grapherbox.hpp
SOURCES += grapherbox.cpp

FORMS   += grapherbox.ui

RESOURCES += $$PWD/icons/grapher.qrc

CONFIG  += grapherbox
