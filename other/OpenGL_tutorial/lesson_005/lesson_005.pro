
TEMPLATE    = app
TARGET      = lesson_005

DEPENDPATH  += \
    /usr/include/GLFW \
    ../ \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += ../common/shader.hpp
SOURCES += ../common/shader.cpp

HEADERS += ../common/texture.hpp
SOURCES += ../common/texture.cpp

SOURCES += src/tutorial05.cpp

LIBS    += -lglfw -lGLEW -lGL
