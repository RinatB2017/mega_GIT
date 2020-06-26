#-------------------------------------------------
#
# Project created by QtCreator 2014-12-08T20:20:49
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SeparatedSquare
TEMPLATE = app

SOURCES += \
    main.cpp \
    Dialog.cpp \
    Scene.cpp \
    Square.cpp

HEADERS  += \
    Dialog.h \
    Scene.h \
    Square.h

FORMS    += Dialog.ui

RESOURCES += \
    Shaders.qrc
