#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT      += network

HEADERS += \
#    processor.hpp \
    tcp_server.hpp

SOURCES += \
#    processor.cpp \
    tcp_server.cpp
