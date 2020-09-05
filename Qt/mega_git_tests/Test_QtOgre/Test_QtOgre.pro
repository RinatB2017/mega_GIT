
TEMPLATE    = app
TARGET      = Test_QtOgre

QT          += opengl x11extras

DEPENDPATH += \
    /usr/include/OGRE/RenderSystems/GL \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$TARGET/rc
}

HEADERS += \
    IOgreEventHandler.hpp \
    QCameraMan.h \
    QOgreWidget.hpp \
    QOgreWidgetDemo.hpp
           
SOURCES += \
    main.cpp \
    QOgreWidget.cpp \
    QOgreWidgetDemo.cpp

LIBS    += -lOIS
LIBS    += -lOgreMain
LIBS    += -lOgreTerrain
LIBS    += -lOgreProperty
LIBS    += -lOgreOverlay
LIBS    += -lOgreVolume
LIBS    += -lOgreRTShaderSystem
LIBS    += -lOgrePaging

LIBS    += -lX11

PKGCONFIG   += OGRE
CONFIG      += link_pkgconfig

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    message(Qt5 = $$QT)
}
else {
    message(Qt4 = $$QT)
}

VPATH = $$INCLUDEPATH
