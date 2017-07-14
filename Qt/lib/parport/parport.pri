#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    /usr/include \
    $$PWD \
    $$PWD/unix
INCLUDEPATH = $$DEPENDPATH

HEADERS += parport.hpp
SOURCES += parport.cpp
