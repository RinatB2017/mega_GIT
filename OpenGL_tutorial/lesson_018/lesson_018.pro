
TEMPLATE    = app
TARGET      = lesson_018

DEPENDPATH  += \
    /usr/include/GLFW \
    ../ \
    $$PWD \
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

HEADERS += ../common/tangentspace.hpp
SOURCES += ../common/tangentspace.cpp

#SOURCES += src/tutorial18_billboards.cpp

SOURCES	+= src/tutorial18_particles.cpp

LIBS    += -lglfw -lGLEW -lGL
