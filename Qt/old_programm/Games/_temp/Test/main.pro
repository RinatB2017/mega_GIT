

TEMPLATE = app
#LANGUAGE = C++
TARGET = Qt_SDL

#PROJECTNAME = SPARTA Video Tracking Demo
CONFIG += qt thread warn_on debug

DEPENDPATH += \
    $$PWD \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

LIBS += -lSDL
LIBS += -lSDL_image

SOURCES += src/main.cpp

LIB_PATH = "../../../lib"
include ($$LIB_PATH/SDLWidget/SDLWidget.pri)

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    message(Qt5 = $$QT)
}
else {
    message(Qt4 = $$QT)
}
