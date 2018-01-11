TEMPLATE = app
CONFIG += c++11

TARGET = adueditor

QT += serialport widgets network

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    modbustcpclient.cpp

HEADERS += \
    mainwindow.h \
    plaintextedit.h \
    modbustcpclient.h \
    modbustcpclient_p.h

FORMS += interface.ui

QT  += serialbus

VPATH = $$INCLUDEPATH
