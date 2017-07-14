TEMPLATE = app
CONFIG += c++11

TARGET = adueditor

QT += serialport widgets network

FORMS += interface.ui

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    modbustcpclient.cpp

HEADERS += \
    mainwindow.h \
    plaintextedit.h \
    modbustcpclient.h \
    modbustcpclient_p.h

include (../../serialbus/serialbus.pri)
