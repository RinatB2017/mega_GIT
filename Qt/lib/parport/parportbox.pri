#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    /usr/include \
    $$PWD \
    $$PWD/ui \
    $$PWD/unix
INCLUDEPATH = $$DEPENDPATH

HEADERS += parportbox.hpp
SOURCES += parportbox.cpp

FORMS   += parportbox.ui
