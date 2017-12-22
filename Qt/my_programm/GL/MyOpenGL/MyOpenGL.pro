#-------------------------------------------------
#
# Project created by QtCreator 2013-09-25T09:11:42
#
#-------------------------------------------------

TARGET      = MyOpenGL
TEMPLATE    = app

PROGRAMM_PATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QT      += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$FOLDER/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$FOLDER/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/rc
}

SOURCES += \
    main.cpp \
    window.cpp \
    myglwidget.cpp

HEADERS += \
    window.h \
    myglwidget.h

FORMS   += window.ui

VPATH = $$INCLUDEPATH
