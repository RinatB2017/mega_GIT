#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Chess

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/chess_mainbox \
    $$PWD/src/chess_mainbox/ui \
    $$PWD/src/new_chessboard
#    $$PWD/src/chessboard
INCLUDEPATH = $$DEPENDPATH

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += SHOW_SIZE
#DEFINES += NO_CHESSBOARD

HEADERS += \
    chessboard.hpp \
    defines.hpp \
    version.hpp \
    chess_mainbox.hpp

SOURCES += \
    chessboard.cpp \
    chess_mainbox.cpp \
    main.cpp

FORMS   += chess_mainbox.ui

RESOURCES   += images/chess.qrc

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
