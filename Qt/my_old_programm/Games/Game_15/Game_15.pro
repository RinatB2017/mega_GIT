#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Game_15

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/game_15_mainbox \
    $$PWD/src/game_15_mainbox/ui \
    $$PWD/src/bone
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON
DEFINES += NO_TOOLBAR
DEFINES += NO_RESIZE
DEFINES += NO_LOG

HEADERS += \
    defines.hpp \
    version.hpp \
    game_15_mainbox.hpp

SOURCES += \
    game_15_mainbox.cpp \
    main.cpp

HEADERS += bone.hpp
SOURCES += bone.cpp

FORMS   += game_15_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
