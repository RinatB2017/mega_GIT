
DEPENDPATH 	+= \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH	= $$DEPENDPATH

HEADERS += worker_fake.hpp
SOURCES += worker_fake.cpp
FORMS   += worker_fake.ui
