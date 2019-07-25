#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += test_glwidget.hpp
SOURCES	+= test_glwidget.cpp

QT *= opengl
LIBS    += -lglut -lGLU
