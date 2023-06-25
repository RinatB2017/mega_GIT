#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/grapherbox \
    $$PWD/src/grapherbox/ui \
    $$PWD/src/grapherbox_options \
    $$PWD/src/grapherbox_options/ui \
    $$PWD/src/colorbutton
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    grapherbox.hpp \
    grapherbox_options.hpp \
    colorbutton.hpp

SOURCES += \
    grapherbox.cpp \
    grapherbox_options.cpp \
    colorbutton.cpp

FORMS   += \
    grapherbox.ui \
    grapherbox_options.ui

RESOURCES += $$PWD/icons/grapher.qrc

CONFIG  += grapherbox
