#-------------------------------------------------
#
# Project created by QtCreator 2014-08-09T13:50:07
#
#-------------------------------------------------
include(../QtYandexApiSettings.pri)

QT       += network testlib

QT       -= gui

TARGET = TestQtYandexApi
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    TQtYandexApi.cpp

HEADERS += \
    Precompiled.h \
    TQtYandexApi.h


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QtYandexApi/release/ -lQtYandexApi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QtYandexApi/debug/ -lQtYandexApi
else:unix: LIBS += -L$$OUT_PWD/../QtYandexApi/ -lQtYandexApi

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../QtYandexApi
