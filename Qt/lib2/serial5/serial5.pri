

DEPENDPATH 	+= \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH	= $$DEPENDPATH

QT  *=  serialport

HEADERS += serialbox5.hpp
SOURCES += serialbox5.cpp
FORMS	+= serialbox5.ui

HEADERS += sendbox5.hpp
SOURCES += sendbox5.cpp

HEADERS += serialbox5_lite.hpp
SOURCES += serialbox5_lite.cpp
FORMS	+= serialbox5_lite.ui

include (../power_knock/power_knock.pri)
