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
    qxmlputget.h \
    AD9106_box.hpp \
    ad9106.hpp

SOURCES += \
    qxmlputget.cpp \
    AD9106_box.cpp \
    ad9106.cpp
    
FORMS   += AD9106_box.ui

RESOURCES   += registers.qrc
