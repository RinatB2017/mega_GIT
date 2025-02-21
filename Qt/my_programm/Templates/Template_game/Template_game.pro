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

QT  += network opengl

greaterThan(QT_MAJOR_VERSION, 5) {
    QT  += openglwidgets
}

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

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include (resources.pri)

VPATH = $$INCLUDEPATH
