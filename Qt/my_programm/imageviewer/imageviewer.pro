QT += widgets
qtHaveModule(printsupport): QT += printsupport

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

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

HEADERS += \
    imageviewer.h

SOURCES += \
    imageviewer.cpp \
    main.cpp

VPATH = $$INCLUDEPATH
