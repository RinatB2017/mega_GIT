
TEMPLATE    = app
TARGET      = lesson_012

DEPENDPATH  += \
    /usr/include/GLFW \
    ../ \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += ../common/shader.hpp
SOURCES += ../common/shader.cpp

HEADERS += ../common/texture.hpp
SOURCES += ../common/texture.cpp

HEADERS += ../common/controls.hpp
SOURCES += ../common/controls.cpp

HEADERS += ../common/objloader.hpp
SOURCES += ../common/objloader.cpp

HEADERS += ../common/vboindexer.hpp
SOURCES += ../common/vboindexer.cpp

SOURCES += src/tutorial12.cpp

LIBS    += -lglfw -lGLEW -lGL
