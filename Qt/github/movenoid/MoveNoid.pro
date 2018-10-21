#-------------------------------------------------
#
# Project created by QtCreator 2017-11-22T14:24:20
#
#-------------------------------------------------

#QMAKE_CXXFLAGS_DEBUG += -pg -no-pie
#MAKE_LFLAGS_DEBUG += -pg -no-pie

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MoveNoid
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$TARGET/rc
}

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    SettingsWindow.cpp \
    AppConfig.cpp \
    GameWindow.cpp \
    Ball.cpp \
    GameField.cpp \
    Barrier.cpp \
    RocketBit.cpp \
    Brick.cpp \
    GraphicsPhysicsItem.cpp \
    ContactListener.cpp \
    CaptureDevice.cpp \
    MoveDetector.cpp \
    CaptureWindow.cpp \
    ReadyPoster.cpp \
    GameOverPoster.cpp \
    MathAn.cpp

HEADERS += \
    MainWindow.h \
    SettingsWindow.h \
    AppConfig.h \
    main.h \
    GameWindow.h \
    Ball.h \
    GameField.h \
    Barrier.h \
    RocketBit.h \
    Brick.h \
    GraphicsPhysicsItem.h \
    ContactListener.h \
    CaptureDevice.h \
    MoveDetector.h \
    CaptureWindow.h \
    ReadyPoster.h \
    GameOverPoster.h \
    MathAn.h

FORMS += \
    MainWindow.ui \
    SettingsWindow.ui \
    GameWindow.ui \
    CaptureWindow.ui \
    ReadyPoster.ui \
    GameOverPoster.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    resource.qrc

# add open CV
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    #PKGCONFIG += box2d
    LIBS    += -lBox2D
}

VPATH = $$INCLUDEPATH
