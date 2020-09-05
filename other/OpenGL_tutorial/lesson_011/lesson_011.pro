
TEMPLATE    = app
TARGET      = lesson_011

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

HEADERS += ../common/text2D.hpp
SOURCES += ../common/text2D.cpp

SOURCES += src/tutorial11.cpp

LIBS    += -lglfw -lGLEW -lGL
