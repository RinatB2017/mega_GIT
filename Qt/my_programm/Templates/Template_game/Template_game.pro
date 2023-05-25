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
    
#    LIBS += c:\Qt\Qt5.14.2\5.14.2\msvc2017_64\lib\Qt5OpenGL.lib
#    LIBS += c:\Qt\Qt5.14.2\5.14.2\mingw73_64\lib\Qt5OpenGL.prl

    LIBS += -lopengl32
#    LIBS += -glu32
}

LIB_PATH = "$$PWD/../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include (resources.pri)

VPATH = $$INCLUDEPATH
