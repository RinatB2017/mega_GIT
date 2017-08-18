#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src/viewer \
    $$PWD/src/viewer/ui \
    $$PWD/src/device \
    $$PWD/src/device/ui \
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    viewer.hpp \
    device.hpp
SOURCES += \
    viewer.cpp \
    device.cpp
FORMS   += \
    viewer.ui \
    device.ui
