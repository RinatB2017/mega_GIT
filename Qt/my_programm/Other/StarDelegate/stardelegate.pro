
DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT += widgets

HEADERS = \
    stardelegate.h \
    stareditor.h \
    starrating.h

SOURCES = \
    main.cpp \
    stardelegate.cpp \
    stareditor.cpp \
    starrating.cpp

VPATH = $$INCLUDEPATH
