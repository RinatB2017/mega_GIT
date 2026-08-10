#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT      += network

HEADERS += udp_server.hpp

SOURCES += udp_server.cpp
