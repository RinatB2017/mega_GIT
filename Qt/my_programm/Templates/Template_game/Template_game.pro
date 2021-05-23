#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template_game

FOLDER  = _Templates

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/game
INCLUDEPATH = $$DEPENDPATH

QT += network opengl

HEADERS += \
    game.hpp \
    defines.hpp \
    map.hpp \
    player.hpp \
    alien.hpp

SOURCES += \
    game.cpp \
    map.cpp \
    player.cpp \
    alien.cpp \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
    
    LIBS += h:\Qt\5.15.2\mingw81_64\lib\libQt5OpenGL.a 
    LIBS += -lopengl32 -glu32
}

LIB_PATH = "$$PWD/../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include (resources.pri)

VPATH = $$INCLUDEPATH
