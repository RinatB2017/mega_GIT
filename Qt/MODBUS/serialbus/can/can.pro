QT += widgets serialport network

TARGET = can
TEMPLATE = app

DEPENDPATH  += $$PWD
INCLUDEPATH = $$DEPENDPATH

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    connectdialog.cpp

HEADERS += \
    mainwindow.h \
    connectdialog.h

FORMS   += \
    mainwindow.ui \
    connectdialog.ui

RESOURCES += can.qrc

include (../serialbus/serialbus.pri)
