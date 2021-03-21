#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Game_15

FOLDER  = old_programm

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "$$PWD/../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/game_15_mainbox/game_15_mainbox.pri)
include (src/bone/bone.pri)

VPATH = $$INCLUDEPATH
