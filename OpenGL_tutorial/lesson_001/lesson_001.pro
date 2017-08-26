
TEMPLATE    = app
TARGET      = lesson_001

DEPENDPATH  += \
    /usr/include/GLFW \
    $$PWD \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

SOURCES += src/tutorial01.cpp

LIBS    += -lglfw -lGLEW -lGL
