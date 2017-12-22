#-------------------------------------------------
#
# Project created by QtCreator 2015-11-16T22:00:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AISimulator
TEMPLATE = app


SOURCES += main.cpp\
        aisimulatorview.cpp \
    aimodel.cpp \
    botai.cpp \
    bot.cpp \
    easyrandomai.cpp \
    smartrandomai.cpp \
    singlememrandomai.cpp \
    accumulatingrandomai.cpp \
    smartsinglememrandomai.cpp

HEADERS  += aisimulatorview.h \
    aimodel.h \
    botai.h \
    bot.h \
    easyrandomai.h \
    smartrandomai.h \
    singlememrandomai.h \
    accumulatingrandomai.h \
    smartsinglememrandomai.h

QMAKE_CXXFLAGS += -std=c++11
