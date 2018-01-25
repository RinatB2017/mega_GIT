#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    multimeter_frame.hpp \
    multimeter_options.hpp
SOURCES += \
    multimeter_frame.cpp \
    multimeter_options.cpp

FORMS += ui/multimeter_options.ui
