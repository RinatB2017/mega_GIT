#-------------------------------------------------
#
# Project created by QtCreator 2013-03-22T21:02:13
#
#-------------------------------------------------

TARGET      = qmqtt-client
TEMPLATE    = app

QT          += network
#CONFIG      += c++11

PROGRAMM_PATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$FOLDER/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$FOLDER/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/rc
}

macx {
    OBJECTS_DIR = build/obj
    MOC_DIR     = build/moc
    UI_DIR      = build/ui
    RCC_DIR     = build/rc
}

win32 {
    TEMP_PATH = "C:\\shm"
    OBJECTS_DIR = $$TEMP_PATH\\my_programm\\$$FOLDER\\$$TARGET\\obj
    MOC_DIR     = $$TEMP_PATH\\my_programm\\$$FOLDER\\$$TARGET\\moc
    UI_DIR      = $$TEMP_PATH\\my_programm\\$$FOLDER\\$$TARGET\\ui
    RCC_DIR     = $$TEMP_PATH\\my_programm\\$$FOLDER\\$$TARGET\\rc

    CONFIG -= debug_and_release #debug_and_release_target
    CONFIG += no_fixpath
}

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    DEFINES += HAVE_QT5
    message(Qt5 = $$QT)
} else {
    message(Qt4 = $$QT)
}

#NOTICE: add DYLD_LIBRARY_PATH to build environment.
#INCLUDEPATH += ../src

LIBS += -lqmqtt

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    connform.cpp \
    pubform.cpp \
    subform.cpp

HEADERS  += \
    mainwindow.h \
    connform.h \
    pubform.h \
    subform.h \
    mqttform.h

FORMS    += \
    mainwindow.ui \
    connform.ui \
    pubform.ui \
    subform.ui

include (src/mqtt/mqtt.pro)
    
VPATH = $$INCLUDEPATH
