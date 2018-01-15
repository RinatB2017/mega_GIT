
QT += serialbus serialport widgets

TARGET      = modbusmaster
TEMPLATE    = app
CONFIG      += c++11

PROGRAMM_PATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    writeregistermodel.cpp

HEADERS  += \
    mainwindow.h \
    settingsdialog.h \
    writeregistermodel.h

FORMS    += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    master.qrc

VPATH = $$INCLUDEPATH
