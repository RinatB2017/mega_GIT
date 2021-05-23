#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/doc \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

unix {
    LIBS    *= -lhidapi-libusb
}
win32 {
    HEADERS += hidapi_win/hidapi.h
    SOURCES += hidapi_win/hid.c
    LIBS    += -lsetupapi
}

HEADERS += \
    HID_device.hpp

SOURCES += \
    HID_device.cpp
    
FORMS   += HID_device.ui
