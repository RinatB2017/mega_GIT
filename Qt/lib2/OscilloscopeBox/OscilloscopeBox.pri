#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/oscilloscope_curve_color \
    $$PWD/src/oscilloscope_curve_color/ui \
    $$PWD/src/oscilloscopebox \
    $$PWD/src/oscilloscopebox/ui \
    $$PWD/src/oscilloscope_controls \
    $$PWD/src/oscilloscope_controls/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    oscilloscope_curve_color.hpp \
    oscilloscopedata.hpp \
    oscilloscopebox.hpp \
    oscilloscope_controls.hpp

SOURCES += \
    oscilloscope_curve_color.cpp \
    oscilloscopebox.cpp \
    oscilloscope_controls.cpp

FORMS += \
    oscilloscope_curve_color.ui \
    oscilloscopebox.ui \
    oscilloscope_controls.ui

RESOURCES += $$PWD/icons/oscilloscope.qrc
