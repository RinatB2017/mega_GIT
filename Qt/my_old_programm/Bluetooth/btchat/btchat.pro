TEMPLATE = app
TARGET = btchat

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT          += bluetooth widgets
CONFIG      += mobility
MOBILITY    += connectivity

SOURCES = \
    main.cpp \
    chat.cpp \
    remoteselector.cpp \
    chatserver.cpp \
    chatclient.cpp

HEADERS = \
    chat.h \
    remoteselector.h \
    chatserver.h \
    chatclient.h

FORMS = \
    chat.ui \
    remoteselector.ui

#include (device/device.pri)
#include (service/service.pri)

VPATH = $$INCLUDEPATH
