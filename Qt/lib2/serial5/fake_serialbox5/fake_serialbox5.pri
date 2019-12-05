
DEPENDPATH 	+= \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH	= $$DEPENDPATH

QT  *=  serialport network

HEADERS += fake_serialbox5.hpp
SOURCES += fake_serialbox5.cpp
