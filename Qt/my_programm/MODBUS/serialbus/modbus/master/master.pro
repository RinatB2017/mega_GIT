QT += serialport widgets network

TARGET = modbusmaster
TEMPLATE = app
CONFIG += c++11

DEPENDPATH  += \
    /usr/include/qt5/QtSerialBus/5.9.3 \
    /usr/include/qt5/QtSerialBus/5.9.3/QtSerialBus
INCLUDEPATH = $$DEPENDPATH

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    settingsdialog.cpp \
    writeregistermodel.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    writeregistermodel.h

FORMS   += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    master.qrc

QT  += serialbus
