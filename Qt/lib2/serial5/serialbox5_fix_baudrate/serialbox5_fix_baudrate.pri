
DEPENDPATH 	+= \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH	= $$DEPENDPATH

QT  *=  serialport

HEADERS += serialbox5_fix_baudrate.hpp
SOURCES += serialbox5_fix_baudrate.cpp
FORMS   += serialbox5_fix_baudrate.ui
