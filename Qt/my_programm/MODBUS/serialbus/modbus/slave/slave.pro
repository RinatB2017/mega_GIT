QT += serialport widgets network

TARGET = modbusslave
TEMPLATE = app
CONFIG += c++11

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
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

VPATH = $$INCLUDEPATH
