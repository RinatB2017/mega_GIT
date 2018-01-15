
TEMPLATE    = app
CONFIG      += c++11
INCLUDEPATH += .
TARGET = adueditor

QT  += \
    serialbus \
    serialport \
    widgets \
    serialbus-private \
    core-private

PROGRAMM_PATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

SOURCES     += \
    main.cpp \
    mainwindow.cpp \
    modbustcpclient.cpp

HEADERS     += \
    mainwindow.h \
    plaintextedit.h \
    modbustcpclient.h \
    modbustcpclient_p.h

FORMS += interface.ui

VPATH = $$INCLUDEPATH
