QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

QMAKE_CXXFLAGS_DEBUG    += -pg
QMAKE_LFLAGS_DEBUG      += -pg

SOURCES += \
        main.cpp
