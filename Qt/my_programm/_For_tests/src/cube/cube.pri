
PROGRAMM_PATH  += \
    $$PWD/src \
    $$PWD/shaders \
    $$PWD/images
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

SOURCES     += \
    cubewidget.cpp \
    geometryengine.cpp

HEADERS     += \
    cubewidget.h \
    geometryengine.h

RESOURCES   += \
    $$PWD/shaders/shaders.qrc \
    $$PWD/images/textures.qrc
