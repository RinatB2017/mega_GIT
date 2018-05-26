#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T22:38:32
#
#-------------------------------------------------

TEMPLATE = app
TARGET = asyncDNS

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/doc
INCLUDEPATH = $$DEPENDPATH

QT       += core network
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

SOURCES += \
    main.cpp \
    buffer.cpp \
    asyncdns.cpp

HEADERS += \
    buffer.h \
    asyncdns.h

VPATH = $$INCLUDEPATH
