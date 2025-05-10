#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

unix {
    DEPENDPATH  += /usr/include/qt6/Qsci
}

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += qscintilla_mainbox.hpp
SOURCES += qscintilla_mainbox.cpp
FORMS   += qscintilla_mainbox.ui
