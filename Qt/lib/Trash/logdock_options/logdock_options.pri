#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    logdock_defines.hpp \
    logdock_options.hpp
SOURCES += \
    logdock_options.cpp

FORMS += logdock_options.ui
