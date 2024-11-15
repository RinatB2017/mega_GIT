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
QT  *= xml

HEADERS += \
    AD9106_box.hpp \
    ad9106.hpp

SOURCES += \
    AD9106_box.cpp \
    ad9106.cpp
    
FORMS   += AD9106_box.ui

RESOURCES   += registers.qrc

include ($$LIB_PATH2/QXmlPutGet/QXmlPutGet.pri)
