#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT      += network

HEADERS += udp_client.hpp
SOURCES += udp_client.cpp
