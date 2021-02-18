#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += adb_widget.hpp
SOURCES += adb_widget.cpp
FORMS   += adb_widget.ui

include ($$LIB_PATH2/icons/digits.pri)
