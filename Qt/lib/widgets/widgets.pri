#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/hexspinbox \
    $$PWD/buttonspinbox \
    $$PWD/labelcombobox \
    $$PWD/labellcdnumber \
    $$PWD/labelbox \
    $$PWD/framebox \
    $$PWD/lcd_knob \
    $$PWD/panel
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    hexspinbox.hpp \
    buttonspinbox.hpp \
    labelcombobox.hpp \
    labellcdnumber.hpp \
    labelbox.hpp \
    framebox.hpp \
    ledspanel.hpp \
    lcd_knob.hpp

SOURCES += \
    hexspinbox.cpp \
    buttonspinbox.cpp \
    labelcombobox.cpp \
    labellcdnumber.cpp \
    labelbox.cpp \
    framebox.cpp \
    ledspanel.cpp \
    lcd_knob.cpp

RESOURCES += framebox.qrc
