#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT      += network

HEADERS += tcp_server.hpp
SOURCES += tcp_server.cpp
FORMS   += tcp_server.ui
