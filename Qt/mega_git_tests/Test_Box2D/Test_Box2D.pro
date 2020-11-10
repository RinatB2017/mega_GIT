#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Box2D

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT  += opengl

#DEFINES += FIXED_SIZE

#DEFINES += NO_TRAYICON
DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_TOOLBAR
#DEFINES += NO_LOG

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

linux {
    DEFINES += BOX2D_231
}

win32 {
#    CONFIG  += use_BOX2D_221
    CONFIG  += use_BOX2D_231
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_Box2D_mainbox/test_Box2D_mainbox.pri)
include (src/world/world.pri)
include ($$LIB_PATH2/Box2D/Box2D.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
