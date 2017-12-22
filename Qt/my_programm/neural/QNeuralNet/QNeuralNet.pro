
TEMPLATE    = app
TARGET      = QNeutralNet

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $DEPENDPATH

QT	+= network

HEADERS += \
    $$PWD/src/mainwindow.h \
    $$PWD/src/network.h \
    $$PWD/src/neuron.h \
    $$PWD/src/race-client.h

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/mainwindow.cpp \
    $$PWD/src/network.cpp \
    $$PWD/src/neuron.cpp \
    $$PWD/src/race-client.cpp

FORMS   += $$PWD/src/ui/mainwindow.ui

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

unix {
    CONFIG(debug,debug|release) {
    #    DESTDIR = bin
        DESTDIR = $$(HOME)/Programming/my_programm_bin/$$TARGET
    } else {
    #    DESTDIR = bin
        DESTDIR = $$(HOME)/Programming/my_programm_bin/$$TARGET
    }
}
win32 {
    DESTDIR = bin
}

QMAKE_CFLAGS    += -std=c++11
QMAKE_CXXFLAGS  += -std=c++11
QMAKE_LFLAGS    += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    message(Qt5 = $$QT)
}
else {
    message(Qt4 = $$QT)
}

VPATH   += $INCLUDEPATH
