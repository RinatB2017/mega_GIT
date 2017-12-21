

DEPENDPATH  += \
    /usr/include/SDL \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += $$PWD/src/SDLWidget.h
SOURCES += $$PWD/src/SDLWidget.cpp

LIBS    += -lGL
LIBS    += -lGLU

LIBS    += -lSDL
LIBS    += -lSDL_image
LIBS    += -lSDL_ttf
LIBS    += -lSDL_mixer

#message ($$PWD)
#message ($$DEPENDPATH)
