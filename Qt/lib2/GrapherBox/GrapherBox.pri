#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/grapherbox \
    $$PWD/src/grapherbox/ui \
    $$PWD/src/grapherbox_options \
    $$PWD/src/grapherbox_options/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    grapherbox.hpp \
    grapherbox_options.hpp

SOURCES += \
    grapherbox.cpp \
    grapherbox_options.cpp

FORMS   += \
    grapherbox.ui \
    grapherbox_options.ui

RESOURCES += $$PWD/icons/grapher.qrc

CONFIG  += grapherbox
