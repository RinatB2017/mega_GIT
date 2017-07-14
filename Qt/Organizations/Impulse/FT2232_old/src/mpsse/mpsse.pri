
DEPENDPATH	+= \
    $$PWD/src \
    $$PWD/test_mpsse
INCLUDEPATH	= $$DEPENDPATH

HEADERS += \
    config.h \
    support.h \
    mpsse.h

SOURCES += \
    fast.c \
    support.c \
    mpsse.c
    
HEADERS += test_mpsse.hpp
SOURCES += test_mpsse.cpp
