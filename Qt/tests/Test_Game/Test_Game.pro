#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Game

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/test_Game_mainbox \
    $$PWD/src/test_Game_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QT  += serialbus

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE
#
DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
DEFINES += SHOW_SIZE
#DEFINES += NO_MENU
#DEFINES += NO_LOG

#DEFINES += ONLY_ENGLISH

#DEFINES += LOG_READ_ONLY

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_Game_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_Game_mainbox.cpp \
    main.cpp

FORMS   += test_Game_mainbox.ui
#FORMS   += test_Game_mainbox_test.ui

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    sprites/rock/rock.qrc \
    sprites/rock2/rock2.qrc \
    sprites/rock3/rock3.qrc \
    sprites/ship/ship.qrc \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

#----------------------------------------------
LIB_PATH = "$$PWD/../../../Qt/lib"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/scenes/scenes.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
