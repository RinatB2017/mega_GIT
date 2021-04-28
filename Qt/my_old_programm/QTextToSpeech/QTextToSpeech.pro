
TEMPLATE    = app
TARGET      = QTextToSpeech

FOLDER  = old_programm

QT  += texttospeech

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    src/mainwindow.h

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

FORMS   += src/ui/mainwindow.ui

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$FOLDER/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$FOLDER/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/rc

    QMAKE_CFLAGS    += -std=c++11
    QMAKE_CXXFLAGS  += -std=c++11
    QMAKE_LFLAGS    += -std=c++11
}

win32 {
    TEMP_PATH = "E:"
    OBJECTS_DIR = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET\\obj
    MOC_DIR     = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET\\moc
    UI_DIR      = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET\\ui
    RCC_DIR     = $$TEMP_PATH/my_programm/$$FOLDER/$$TARGET\\rc
}

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    message(Qt5 = $$QT)
}
else {
    message(Qt4 = $$QT)
}
