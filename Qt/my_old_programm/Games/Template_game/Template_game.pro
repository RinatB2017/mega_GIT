#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template_game

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/game
INCLUDEPATH = $$DEPENDPATH

QT += network opengl

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    game.hpp \
    defines.hpp \
    version.hpp \
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
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/turbo.pri)
include (resources.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
