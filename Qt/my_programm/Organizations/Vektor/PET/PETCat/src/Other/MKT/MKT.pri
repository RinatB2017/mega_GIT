#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/MKT_box \
    $$PWD/MKT_box/ui
INCLUDEPATH = $$DEPENDPATH

unix {
    LIBS    += -lmodbus
}
win32 {
    include (libmodbus_win/libmodbus_win.pri)
}

HEADERS += MKT_box.hpp
SOURCES += MKT_box.cpp
FORMS   += MKT_box.ui
