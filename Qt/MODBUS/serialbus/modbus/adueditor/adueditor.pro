TEMPLATE = app
CONFIG += c++11

TARGET = adueditor

QT += serialport widgets network

DEPENDPATH  += \
    /usr/include/qt5/QtSerialBus/5.9.3 \
    /usr/include/qt5/QtSerialBus/5.9.3/QtSerialBus \
    /usr/include/qt5/QtCore/5.9.3/QtCore
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
