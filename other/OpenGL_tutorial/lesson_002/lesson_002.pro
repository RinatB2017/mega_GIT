
TEMPLATE    = app
TARGET      = lesson_002

DEPENDPATH  += \
    /usr/include/GLFW \
    ../ \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += ../common/shader.hpp
SOURCES += ../common/shader.cpp

SOURCES += src/tutorial02.cpp

LIBS    += -lglfw -lGLEW -lGL
