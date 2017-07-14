#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/oscilloscopebox \
    $$PWD/src/oscilloscopebox/ui \
    $$PWD/src/oscilloscope_controls \
    $$PWD/src/oscilloscope_controls/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    oscilloscopedata.hpp \
    oscilloscopebox.hpp \
    oscilloscope_controls.hpp

SOURCES += \
    oscilloscopebox.cpp \
    oscilloscope_controls.cpp

FORMS += \
    oscilloscopebox.ui \
    oscilloscope_controls.ui

RESOURCES += $$PWD/icons/oscilloscope.qrc
