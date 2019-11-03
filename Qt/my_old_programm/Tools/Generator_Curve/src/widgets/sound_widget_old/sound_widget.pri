#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += sound_widget.hpp
SOURCES += sound_widget.cpp
FORMS   += sound_widget.ui

HEADERS += generator.h
SOURCES += generator.cpp
