TARGET      = "QRCode Generator"
TEMPLATE    = app

include (conf/conf.pri)

PROGRAMM_PATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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

DESTDIR = bin

SOURCES += \
    mainwindow.cpp \
    main.cpp

HEADERS  += \
    mainwindow.h

FORMS    += \
    mainwindow.ui

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}
include ($$LIB_PATH2/QRCodeGenerator/QRCodeGenerator.pri)

VPATH = $$INCLUDEPATH
