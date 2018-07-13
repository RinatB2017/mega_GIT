TARGET = btscanner

INCLUDEPATH += ../src/connectivity/bluetooth ../src/global
DEPENDPATH  += ../src/connectivity/bluetooth ../src/global

#include(../mobility_examples.pri)

#QT          += bluetooth widgets
CONFIG += mobility
MOBILITY = connectivity
TEMPLATE = app

SOURCES = \
    main.cpp \
    device.cpp \
    service.cpp

HEADERS = \
    device.h \
    service.h

FORMS = \
    device.ui \
    service.ui


