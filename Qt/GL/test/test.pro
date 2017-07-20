QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = test2
TEMPLATE    = app

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
