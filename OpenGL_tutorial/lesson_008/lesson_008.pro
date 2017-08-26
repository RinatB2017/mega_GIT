
TEMPLATE    = app
TARGET      = lesson_008

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

SOURCES += src/tutorial08.cpp

LIBS    += -lglfw -lGLEW -lGL
