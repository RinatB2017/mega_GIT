
QT      += serialbus serialport widgets

TARGET      = modbusslave
TEMPLATE    = app
CONFIG      += c++11

PROGRAMM_PATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

SOURCES     += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp

HEADERS     += \
    mainwindow.h \
    settingsdialog.h

FORMS       += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += slave.qrc

VPATH = $$INCLUDEPATH
