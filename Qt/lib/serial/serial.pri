#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD \
    $$PWD/serialbox \
    $$PWD/serialbox/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    sendbox.hpp \
    serialbox.hpp

SOURCES += \
    sendbox.cpp \
    serialbox.cpp

FORMS += serialbox.ui

#TODO: here in future replace
contains( CONFIG, dll ) {
    win32:DEFINES += QSERIALDEVICE_SHARED
}

include (../power_knock/power_knock.pri)
include ($$PWD/qserialdevice/qserialdevice.pri)
include ($$PWD/qserialdeviceenumerator/qserialdeviceenumerator.pri)
DEPENDPATH = $$INCLUDEPATH

win32 {
    LIBS += -lsetupapi -luuid -ladvapi32
}
unix:!macx {
    LIBS += -ludev
}
macx {
    LIBS += -framework IOKit -framework CoreFoundation
}
