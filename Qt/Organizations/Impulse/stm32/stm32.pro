
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET  = stm32_template

OBJECTS_DIR = obj
DESTDIR     = bin

INCLUDEPATH = \
    src
DEPENDPATH = $$INCLUDEPATH

include(stm32.pri)

SOURCES += main.c

VPATH = $$DEPENDPATH
