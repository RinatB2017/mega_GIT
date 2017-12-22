#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src

greaterThan(QT_MAJOR_VERSION, 4) {
    DEPENDPATH  += $$PWD/src/ui5
} else {
    DEPENDPATH  += $$PWD/src/ui4
}

INCLUDEPATH = $$DEPENDPATH

HEADERS += serial.hpp
SOURCES += serial.cpp
FORMS   += serial.ui
