
TEMPLATE = app
TARGET = QOgreWidget

QT  += opengl

DEPENDPATH += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/QOgreWidget
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    QOgreWidget.hpp

SOURCES += \
    main.cpp \
    QOgreWidget.cpp

HEADERS += QtOgreApplication.hpp
SOURCES += QtOgreApplication.cpp

LIBS    += -lOgreMain
LIBS    += -lOgreTerrain
LIBS    += -lOgreProperty
LIBS    += -lOgreOverlay
LIBS    += -lOgreVolume
LIBS    += -lOgreRTShaderSystem
LIBS    += -lOgrePaging

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
