#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/histogrambox \
    $$PWD/src/histogrambox/ui \
    $$PWD/src/histogram_options \
    $$PWD/src/histogram_options/ui \
    $$PWD/src/histogram_settings \
    $$PWD/src/histogram_curvebox
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    curvedata.hpp \
    histogram_options.hpp \
    histogram_settings.hpp \
    histogram_curvebox.hpp \
    histogrambox.hpp

SOURCES += \
    histogrambox.cpp \
    histogram_options.cpp \
    histogram_settings.cpp \
    histogram_curvebox.cpp

FORMS += \
    histogrambox.ui \
    histogram_options.ui

RESOURCES += $$PWD/icons/histogram.qrc

CONFIG  += histogrambox
