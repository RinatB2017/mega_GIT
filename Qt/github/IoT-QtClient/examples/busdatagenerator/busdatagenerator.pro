QT += core network
QT -= gui

CONFIG += c++11

TARGET = busdatagenerator
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    buscreator.cpp \
    bus.cpp

HEADERS += \
    buscreator.h \
    bus.h

INCLUDEPATH += $$PWD/../../src/ $$PWD
LIBS += -L$$OUT_PWD/../../src/debug/ -liot
