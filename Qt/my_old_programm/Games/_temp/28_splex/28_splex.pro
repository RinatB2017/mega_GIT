
TEMPLATE    = app
TARGET      = 28_splex

DEPENDPATH += \
    /usr/include/GL \
    /usr/include/GLU \
    /usr/include/SDL \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    src/edit.h \
    src/effects.h \
    src/graph.h \
    src/level.h \
    src/obj.h \
    src/other.h \
    src/player.h \
    src/sm2.h \
    src/var.h

SOURCES += src/splex.cpp

OTHER_FILES	+= \
    src/splex.cfg \
    src/level.lst    

QT      = opengl

LIBS    += -lGL
LIBS    += -lGLU

LIBS    += -lstdc++
LIBS    += -lm

LIBS    += -lSDL
LIBS    += -lSDL_ttf
LIBS    += -lSDL_mixer
LIBS    += -lSDL_image
LIBS    += -lSDL_gfx
