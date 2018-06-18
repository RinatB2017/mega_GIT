#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

#QT *= webkit network
QT  += webenginewidgets

HEADERS += webview.hpp
SOURCES += webview.cpp
