#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/.. \
    $$PWD \
    $$PWD/src
INCLUDEPATH += \
    $$PWD/.. \
    $$PWD \
    $$PWD/src

HEADERS += scan_toolbar.hpp
SOURCES += scan_toolbar.cpp

HEADERS += scan_tooldock.hpp
SOURCES += scan_tooldock.cpp

HEADERS += mymainwindow.hpp
SOURCES += mymainwindow.cpp

include (log_options/log_options.pri)
include (log/log.pri)

VPATH = $$INCLUDEPATH
