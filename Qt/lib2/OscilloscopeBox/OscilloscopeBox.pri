#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/color_curve \
    $$PWD/src/color_curve/ui \
    $$PWD/src/oscilloscopebox \
    $$PWD/src/oscilloscopebox/ui \
    $$PWD/src/oscilloscope_controls \
    $$PWD/src/oscilloscope_controls/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    color_curve.hpp \
    oscilloscopedata.hpp \
    oscilloscopebox.hpp \
    oscilloscope_controls.hpp

SOURCES += \
    color_curve.cpp \
    oscilloscopebox.cpp \
    oscilloscope_controls.cpp

FORMS += \
    color_curve.ui \
    oscilloscopebox.ui \
    oscilloscope_controls.ui

RESOURCES += $$PWD/icons/oscilloscope.qrc
