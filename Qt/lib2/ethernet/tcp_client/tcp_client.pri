#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT      += network

HEADERS += tcp_client.hpp

SOURCES += tcp_client.cpp
