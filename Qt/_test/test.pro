#-------------------------------------------------
#
# Project created by QtCreator 2017-07-22T20:15:50
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT          += network webkit webkitwidgets

TARGET      = test
TEMPLATE    = app

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.hpp

FORMS += \
        mainwindow.ui

VPATH = $$INCLUDEPATH

message ($$QT)
