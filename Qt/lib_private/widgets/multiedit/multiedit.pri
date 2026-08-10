#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += multiedit.hpp
SOURCES += multiedit.cpp
FORMS   += multiedit.ui

include ($$LIB_PATH2/icons/digits.pri)
