QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = test2
TEMPLATE    = app

PROGRAMM_PATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QT          += opengl

LIBS        += -lglut -lGLU -lGL

SOURCES     += \
    main.cpp \
    mainwindow.cpp \
    oglwidget.cpp

HEADERS     += \
    mainwindow.h \
    oglwidget.h

FORMS       += mainwindow.ui

VPATH = $$INCLUDEPATH
