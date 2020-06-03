#---
# https://habr.com/ru/post/218389/
#---

TARGET = d3widgets
TEMPLATE = app

QT       += core gui webkit qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

contains(QT_VERSION, ^5.*) {
   QT += webkitwidgets
}

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
    TEMP_PATH = "X:"
    OBJECTS_DIR = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET\\obj
    MOC_DIR     = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET\\moc
    UI_DIR      = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET\\ui
    RCC_DIR     = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET\\rc

    CONFIG -= debug_and_release #debug_and_release_target
    CONFIG += no_fixpath

    DEFINES += WIN32_LEAN_AND_MEAN
}

SOURCES += \
    main.cpp \
    base/d3viewer.cpp \
    base/d3webpage.cpp \
    charts/pie/dependencywheelwidget.cpp

HEADERS  += \
    base/d3viewer.h \
    base/d3webpage.h \
    charts/pie/dependencywheelwidget.h

RESOURCES += \
    resources/res.qrc
