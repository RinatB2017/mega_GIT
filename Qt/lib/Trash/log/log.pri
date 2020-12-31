#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

CONFIG(debug, debug|release) {
} else {
    DEFINES += NO_LOG_DEBUG
    DEFINES += NO_LOG_TRACE
}

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += logbox.hpp
SOURCES += logbox.cpp
