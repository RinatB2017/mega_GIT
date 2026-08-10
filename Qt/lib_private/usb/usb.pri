#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

unix {
    LIBS    += -lhidapi-libusb
    LIBS    += -lusb-1.0
}
win32 {
    include ($$PWD/hidapi_win/hidapi_win.pri)
    include ($$PWD/libusb_win/libusb_win.pri)

    LIBS    += -L$$PWD/lib
    LIBS    += -lsetupapi
    LIBS    += -lusb-1.0
}

include ($$PWD/usb/usb.pri)
