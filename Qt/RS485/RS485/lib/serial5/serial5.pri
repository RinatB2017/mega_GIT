

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

include (../power_knock/power_knock.pri)
