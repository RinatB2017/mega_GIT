QT += core
QT -= gui

INCLUDEPATH += \
    ../common \
    $$PWD
DEPENDPATH  = $$INCLUDEPATH

#DEFINES += HOME

CONFIG  += c++11
QT += network

TARGET  = pet_test
CONFIG  += console
CONFIG  -= app_bundle

TEMPLATE = app

HEADERS += ../common/pet.h

SOURCES += main.cpp

include (../common/PET/PET.pri)
