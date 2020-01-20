#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += dock_position.hpp
SOURCES += dock_position.cpp
FORMS   += dock_position.ui

RESOURCES   += $$LIB_PATH2/pictures/arrow.qrc
