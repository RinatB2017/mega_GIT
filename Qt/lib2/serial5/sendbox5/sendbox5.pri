
DEPENDPATH 	+= \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH	= $$DEPENDPATH

QT  *=  serialport

HEADERS += sendbox5.hpp
SOURCES += sendbox5.cpp
FORMS   += sendbox5.ui
