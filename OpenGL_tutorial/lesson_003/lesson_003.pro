
TEMPLATE    = app
TARGET      = lesson_003

DEPENDPATH  += \
    /usr/include/GLFW \
    ../ \
    $$PWD \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += ../common/shader.hpp
SOURCES += ../common/shader.cpp

SOURCES += src/tutorial03.cpp

LIBS    += -lglfw -lGLEW -lGL
