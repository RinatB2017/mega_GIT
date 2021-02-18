#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT      += network

HEADERS += tcp_local_server.hpp
SOURCES += tcp_local_server.cpp
