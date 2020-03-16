
DEPENDPATH 	+= $$PWD/src
INCLUDEPATH	= $$DEPENDPATH

HEADERS += serialwidget.hpp
SOURCES += serialwidget.cpp

include (worker_fake/worker_fake.pri)
