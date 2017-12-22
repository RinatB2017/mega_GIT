#-------------------------------------------------
#
# Project created by QtCreator 2014-08-09T21:51:12
#
#-------------------------------------------------
include(../QtYandexApiSettings.pri)

QT       += core network

QT       -= gui

TARGET = YandexDictionaryExample
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QtYandexApi/release/ -lQtYandexApi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QtYandexApi/debug/ -lQtYandexApi
else:unix: LIBS += -L$$OUT_PWD/../QtYandexApi/ -lQtYandexApi

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../QtYandexApi

HEADERS += \
    Precompiled.h
