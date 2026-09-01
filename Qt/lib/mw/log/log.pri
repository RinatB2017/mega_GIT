#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

CONFIG(debug, debug|release) {
} else {
    DEFINES += DEF_NO_LOG_DEBUG
    DEFINES += DEF_NO_LOG_TRACE
}

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += logbox.hpp
SOURCES += logbox.cpp
