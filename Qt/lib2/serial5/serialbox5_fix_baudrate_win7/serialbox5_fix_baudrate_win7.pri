
DEPENDPATH 	+= \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH	= $$DEPENDPATH

QT  *=  serialport

HEADERS += serialbox5_fix_baudrate_win7.hpp
SOURCES += serialbox5_fix_baudrate_win7.cpp
FORMS   += serialbox5_fix_baudrate_win7.ui

HEADERS += serialbox5_thread.hpp
SOURCES += serialbox5_thread.cpp

versionAtLeast(QT_VERSION, 5.13.0) {
    !versionAtMost(QT_VERSION, 5.13.1) {
        error("Use only in Qt version 5.13.1")
    }
}
