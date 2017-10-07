
TEMPLATE    = app
TARGET      = QTextToSpeech

FOLDER  = old_programm

DEPENDPATH  += \
    /usr/include/qt5/QtCore/5.9.2 \
    /usr/include/qt5/QtCore/5.9.2/QtCore \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    src/mainwindow.h

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

FORMS   += src/ui/mainwindow.ui

include (tts/tts.pri)

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$TARGET/rc
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
