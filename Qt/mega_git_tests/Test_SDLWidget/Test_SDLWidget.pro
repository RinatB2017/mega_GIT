#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_SDLWidget

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE

#
DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_MENU
#DEFINES += NO_LOG

#DEFINES += ONLY_ENGLISH

#DEFINES += LOG_READ_ONLY
#DEFINES += NEED_CORRECT_LOG

DEFINES += PROGRAMM_IN_UTF8
DEFINES += SINGLE_APP

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

RESOURCES   += image/image.qrc
OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/SDLWidget/SDLWidget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
