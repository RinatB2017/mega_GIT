#-------------------------------------------------
#
# Project created by QtCreator 2014-12-20T22:18:55
#
#-------------------------------------------------

-Wno-reorder

QT += core gui \
        opengl \
        opengl widgets \
        network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = Habrahabr
TEMPLATE = app

INCLUDEPATH +=  ./Include \
                ./Include/ComPort \
                ./Include/Console \
                ./Include/LED


SOURCES += \
    main.cpp \
    MainWindow.cpp \    
    Source/ComPort/ComPortThread.cpp \
    Source/ComPort/ModelComPort.cpp \
    Source/ComPort/PresenterComPort.cpp \
    Source/Console/CLI.cpp \
    Source/LED/LED.cpp

HEADERS  += \
    MainWindow.h \    
    Include/ComPort/Commands.h \
    Include/ComPort/ComPortThread.h \
    Include/ComPort/IViewComPort.h \
    Include/ComPort/ModelComPort.h \
    Include/ComPort/PresenterComPort.h \
    Include/Console/CLI.h \
    Include/LED/LED.h

FORMS    += MainWindow.ui
