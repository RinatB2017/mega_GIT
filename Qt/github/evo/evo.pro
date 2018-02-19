#-------------------------------------------------
#
# Project created by QtCreator 2012-05-22T11:09:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

TARGET      = evo
TEMPLATE    = app

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$TARGET/rc
}

win32 {
    OBJECTS_DIR = build
    MOC_DIR = build
    UI_DIR  = build
    RCC_DIR = build
}

SOURCES     += \
    main.cpp \
    mainwindow.cpp \
    world.cpp \
    animal.cpp \
    map.cpp

HEADERS     += \
    mainwindow.h \
    world.h \
    animal.h \
    common.h \
    map.h

FORMS       += mainwindow.ui

VPATH = $$INCLUDEPATH
