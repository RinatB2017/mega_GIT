#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

unix {
    LIBS    += -lhidapi-libusb
    LIBS    += -lusb-1.0
}
win32 {
    include ($$PWD/hidapi_win/hidapi_win.pri)
    include ($$PWD/libusb_win/libusb_win.pri)

    LIBS    += -lsetupapi
    LIBS    += -L$$PWD/lib
    LIBS    += -lusb-1.0
}

include ($$PWD/usb/usb.pri)
