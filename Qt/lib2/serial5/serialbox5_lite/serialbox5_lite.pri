
DEPENDPATH 	+= \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH	= $$DEPENDPATH

QT  *=  serialport

HEADERS += serialbox5_lite.hpp
SOURCES += serialbox5_lite.cpp
FORMS   += serialbox5_lite.ui
