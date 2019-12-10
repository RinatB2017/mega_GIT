#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/oscilloscopebox_controls \
    $$PWD/src/oscilloscopebox_controls/ui \
    $$PWD/src/oscilloscopebox \
    $$PWD/src/oscilloscopebox/ui \
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    oscilloscopebox_controls.hpp \
    oscilloscopedata.hpp \
    oscilloscopebox.hpp

SOURCES += \
    oscilloscopebox_controls.cpp \
    oscilloscopebox.cpp \

FORMS += \
    oscilloscopebox_controls.ui \
    oscilloscopebox.ui \

RESOURCES += $$PWD/icons/oscilloscope.qrc
