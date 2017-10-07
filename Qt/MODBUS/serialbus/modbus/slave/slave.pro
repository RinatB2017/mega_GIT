QT += serialport widgets network

TARGET = modbusslave
TEMPLATE = app
CONFIG += c++11

DEPENDPATH  += \
    /usr/include/qt5/QtSerialBus/5.9.2 \
    /usr/include/qt5/QtSerialBus/5.9.2/QtSerialBus
INCLUDEPATH = $$DEPENDPATH

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    settingsdialog.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h

FORMS   += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += slave.qrc

QT  += serialbus
