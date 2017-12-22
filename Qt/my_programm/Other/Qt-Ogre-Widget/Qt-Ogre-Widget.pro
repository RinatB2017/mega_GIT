
TEMPLATE    = app
TARGET      = Qt-Ogre-Widget

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT  += x11extras
QT  += opengl
QT  += widgets

HEADERS += \
    PreProcessor.h \
    QOgreApplication.h \
    QOgreWidget.h \
    QOgreWindow.h \
    QtOgre.h \
    WindowEventListener.h

SOURCES += \
    main.cpp \
    QOgreWidget.cpp \
    QOgreWindow.cpp

#LIBS    += -lOgreMain
#LIBS    += -lOgreTerrain
#LIBS    += -lOgreProperty
#LIBS    += -lOgreOverlay
#LIBS    += -lOgreVolume
#LIBS    += -lOgreRTShaderSystem
#LIBS    += -lOgrePaging

PKGCONFIG   += OGRE
CONFIG      += link_pkgconfig

VPATH = $$INCLUDEPATH
