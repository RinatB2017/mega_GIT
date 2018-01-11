QT += serialport widgets network

TARGET = modbusmaster
TEMPLATE = app
CONFIG += c++11

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
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

VPATH = $$INCLUDEPATH
