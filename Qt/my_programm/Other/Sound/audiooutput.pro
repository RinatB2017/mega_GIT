
TEMPLATE    = app
TARGET      = audiooutput

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT  += multimedia widgets

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$FOLDER/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$FOLDER/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/rc
}

linux {
    DESTDIR = $$(HOME)/Programming/my_programm_bin/$$FOLDER/$$TARGET
}
macx {
    DESTDIR = bin
}
win32 {
    DESTDIR = C:/Programming/my_programm_bin/$$FOLDER/$$TARGET
}


HEADERS     = \
    audiooutput.h \
    generator.h

SOURCES     = \
    audiooutput.cpp \
    generator.cpp \
    main.cpp

VPATH = $$INCLUDEPATH
