
TEMPLATE    = app
TARGET      = SBGlass

DESTDIR     = ../Debug
QT          += core gui
CONFIG      += debug

#DEFINES     += QT_LARGEFILE_SUPPORT

INCLUDEPATH += \
    $$PWD/src \
    $$PWD/src/sbglass \
    $$PWD/src/sbcontrol \
    $$PWD/src/www \
    $$PWD/src/ui
DEPENDPATH  = $$INCLUDEPATH

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$TARGET/rc
}

win32 {
    TEMP_PATH = "C:\\temp"
    OBJECTS_DIR = $$TEMP_PATH\\my_programm\\$$TARGET\\obj
    MOC_DIR     = $$TEMP_PATH\\my_programm\\$$TARGET\\moc
    UI_DIR      = $$TEMP_PATH\\my_programm\\$$TARGET\\ui
    RCC_DIR     = $$TEMP_PATH\\my_programm\\$$TARGET\\rc
}
CONFIG(debug,debug|release) {
    DESTDIR = bin
} else {
    DESTDIR = bin
}

include(SBGlass.pri)

unix {
    QMAKE_CFLAGS    += -std=c++11
    QMAKE_CXXFLAGS  += -std=c++11
    QMAKE_LFLAGS    += -std=c++11
}

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    message(Qt5 = $$QT)
}

