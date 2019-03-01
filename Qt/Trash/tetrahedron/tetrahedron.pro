
TEMPLATE    = app
QT          += opengl

PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

HEADERS     = \
    tetrahedron.h

SOURCES     = \
    main.cpp \
    tetrahedron.cpp
                
LIBS        += -lglut -lGLU

VPATH = $$INCLUDEPATH
