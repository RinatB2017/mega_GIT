
TEMPLATE    = app
TARGET      = audiooutput

DEPENDPATH  += \
    $$PWD \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT += multimedia widgets

HEADERS     = \
    audiooutput.h \
    generator.h

SOURCES     = \
    audiooutput.cpp \
    generator.cpp \
    main.cpp

VPATH = $$INCLUDEPATH
