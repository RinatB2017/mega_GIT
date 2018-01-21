
TARGET      = Test_PGP
TEMPLATE    = app

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$TARGET/rc
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS     += CryptWin.h

SOURCES     += \
    main.cpp \
    CryptWin.cpp

FORMS       += CryptWin.ui

CONFIG      += crypto

VPATH = $$INCLUDEPATH
