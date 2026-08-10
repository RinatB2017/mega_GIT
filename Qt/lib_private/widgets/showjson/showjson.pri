#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += showjson.hpp
SOURCES += showjson.cpp
FORMS   += showjson.ui

include (qjsonmodel/qjsonmodel.pri)
