#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Box2D

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/Test_Box2D_mainbox \
    $$PWD/src/Test_Box2D_mainbox/ui \
    $$PWD/src/world
INCLUDEPATH = $$DEPENDPATH

QT  += opengl

#DEFINES += FIXED_SIZE

#DEFINES += NO_TRAYICON
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TOOLBAR
#DEFINES += NO_LOG

DEFINES += SAVE_WIDGETS_CHECKBOX

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    Test_Box2D_mainbox.hpp \
    listener.hpp \
    world.hpp

SOURCES += \
    Test_Box2D_mainbox.cpp \
    listener.cpp \
    world.cpp \
    main.cpp

FORMS   += Test_Box2D_mainbox.ui

#RESOURCES += icons/temp.qrc

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

win32 {
    #include ($$LIB_PATH2/Box2D/Box2D.pri)
}
unix {
    LIBS    += -lBox2D
}

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
