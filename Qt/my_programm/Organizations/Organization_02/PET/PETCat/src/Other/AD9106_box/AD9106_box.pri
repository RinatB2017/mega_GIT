#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD \
    $$PWD/doc \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

unix {
    LIBS    += -lhidapi-libusb
    #SOURCES += src/hidapi_fake/hid_fake.c
}
win32 {
    HEADERS += src/hidapi_win/hidapi.h
    SOURCES += src/hidapi_win/hid.c
    LIBS    += -lsetupapi
}

HEADERS += \
    qxmlputget.h \
    AD9106_box.hpp \
    ad9106.hpp

SOURCES += \
    qxmlputget.cpp \
    AD9106_box.cpp \
    ad9106.cpp
    
FORMS   += AD9106_box.ui

RESOURCES   += registers.qrc
