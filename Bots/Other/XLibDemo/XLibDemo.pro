QT += core
QT -= gui

CONFIG += c++11

TARGET = XLibDemo
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

LIBS += -lX11
