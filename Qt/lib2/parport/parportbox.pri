#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/ui \
    $$PWD/unix
INCLUDEPATH = $$DEPENDPATH

HEADERS += parportbox.hpp
SOURCES += parportbox.cpp

FORMS   += parportbox.ui
