#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T22:12:30
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = QtYandexApi
TEMPLATE = lib

DEFINES += QTYANDEXAPI_LIBRARY
include(../QtYandexApiSettings.pri)

SOURCES += \
    QtYandexDictionary.cpp \
    QtYaWord.cpp \
    QtYaWordTranslation.cpp \
    QtYaLanguages.cpp \
    QtYaError.cpp \
    QtYaTranslatedWord.cpp \
    QtYaTranslationGettor.cpp \
    QtYandexLanguagesGettor.cpp \
    QtYandexApi.cpp

HEADERS +=\
        qtyandexapi_global.h \
    Precompiled.h \
    QtYandexDictionary.h \
    QtYaWord.h \
    QtYaWordTranslation.h \
    QtYaLanguages.h \
    QtYaError.h \
    QtYaTranslatedWord.h \
    QtYaTranslationGettor.h \
    QtYandexLanguagesGettor.h \
    QtYandexApi.h \
    Common.h
