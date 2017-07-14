#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/grapherbox \
    $$PWD/src/grapherbox/ui \
    $$PWD/src/grapherbox_options \
    $$PWD/src/grapherbox_options/ui \
    $$PWD/src/grapher_settings \
    $$PWD/src/grapher_curvebox \
    $$PWD/src/curve_corrects
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    curvedata.hpp \
    grapherbox.hpp \
    grapherbox_options.hpp \
    grapher_settings.hpp \
    grapher_curvebox.hpp \
    curve_corrects.hpp

SOURCES += \
    grapherbox.cpp \
    grapherbox_options.cpp \
    grapher_settings.cpp \
    grapher_curvebox.cpp \
    curve_corrects.cpp

FORMS += \
    grapherbox.ui \
    grapherbox_options.ui

RESOURCES += $$PWD/icons/grapher.qrc
