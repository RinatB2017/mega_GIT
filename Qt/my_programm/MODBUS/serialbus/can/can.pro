QT += widgets serialport network

TARGET      = can
TEMPLATE    = app
CONFIG      += c++11

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

SOURCES     += \
    main.cpp \
    mainwindow.cpp \
    connectdialog.cpp

HEADERS     += \
    mainwindow.h \
    connectdialog.h

FORMS       += \
    mainwindow.ui \
    connectdialog.ui

RESOURCES   += can.qrc

QT          += serialbus

VPATH = $$INCLUDEPATH
