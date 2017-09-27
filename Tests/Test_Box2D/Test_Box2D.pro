#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Box2D

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui \
    $$PWD/src/world
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

QT  += opengl

#DEFINES += FIXED_SIZE
#DEFINES += FLAG_RESIZE
#DEFINES += NO_TRAYICON
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TOOLBAR
#DEFINES += NO_LOG

DEFINES += SAVE_WIDGETS_CHECKBOX

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp \
    listener.hpp \
    world.hpp

SOURCES += \
    mainbox.cpp \
    listener.cpp \
    world.cpp \
    main.cpp

FORMS += mainbox.ui

#RESOURCES += icons/temp.qrc

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../Qt/lib"
include ($$LIB_PATH/meta/mainwindow.pri)

#include ($$LIB_PATH/Box2D/Box2D.pri)
LIBS    += -lBox2D

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
