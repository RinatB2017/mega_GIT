QT += widgets serialport network

TARGET      = can
TEMPLATE    = app
CONFIG      += c++11

DEPENDPATH  += \
    /usr/include/qt5/QtSerialBus/5.9.3 \
    /usr/include/qt5/QtSerialBus/5.9.3/QtSerialBus \
    /usr/include/qt5/QtCore/5.9.3/QtCore \
    $$PWD
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
