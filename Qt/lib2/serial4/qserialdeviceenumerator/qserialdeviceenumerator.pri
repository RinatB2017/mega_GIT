
HEADERS += \
    $$PWD/serialdeviceenumerator.h \
    $$PWD/serialdeviceenumerator_p.h

SOURCES += \
    $$PWD/serialdeviceenumerator.cpp

win32 {
    SOURCES += $$PWD/serialdeviceenumerator_p_win.cpp
}
unix {
#    HEADERS += $$PWD/../qserialdevice/ttylocker.h
#    SOURCES += $$PWD/../qserialdevice/ttylocker.cpp
    macx {
        SOURCES += $$PWD/serialdeviceenumerator_p_mac.cpp
    }
    !macx {
        SOURCES += $$PWD/serialdeviceenumerator_p_unix.cpp
    }
}

INCLUDEPATH += $$PWD
