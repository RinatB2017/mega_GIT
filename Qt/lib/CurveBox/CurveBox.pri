#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/curvebox \
    $$PWD/src/curvebox/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += curvebox.hpp
SOURCES += curvebox.cpp
FORMS   += ui/curvebox.ui

HEADERS += curvedata.hpp

RESOURCES += $$PWD/icons/grapher.qrc
