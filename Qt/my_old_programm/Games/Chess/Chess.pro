#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Chess

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
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
    version.hpp

SOURCES += \
    chessboard.cpp \
    main.cpp

RESOURCES   += images/chess.qrc

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "$$PWD/../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/chess_mainbox/chess_mainbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
