
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
