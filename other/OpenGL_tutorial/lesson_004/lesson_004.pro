
TEMPLATE    = app
TARGET      = lesson_004

DEPENDPATH  += \
    /usr/include/GLFW \
    ../ \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += ../common/shader.hpp
SOURCES += ../common/shader.cpp

SOURCES += src/tutorial04.cpp

LIBS    += -lglfw -lGLEW -lGL
