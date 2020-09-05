
PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$TARGET/rc
}

macx {
    OBJECTS_DIR = build/obj
    MOC_DIR     = build/moc
    UI_DIR      = build/ui
    RCC_DIR     = build/rc
}

DESTDIR = bin

QT += widgets

HEADERS       = \
    classwizard.hpp \
    defines.hpp

SOURCES       = \
    classwizard.cpp \
    main.cpp

RESOURCES     = \
    classwizard.qrc

include (src/pages/pages.pri)

VPATH = $$INCLUDEPATH
