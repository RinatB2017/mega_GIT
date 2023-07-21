#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += test_glwidget.hpp
SOURCES	+= test_glwidget.cpp

QT *= opengl

#unix {
#    LIBS    += -lglut -lGLU
#}

win32 {
    //LIBS += h:\Qt\5.15.2\mingw81_64\lib\libQt5OpenGL.a
    LIBS += c:\Qt\Qt5.14.2\5.14.2\mingw73_64\lib\libQt5OpenGL.a
    LIBS += -lopengl32 -glu32
}
