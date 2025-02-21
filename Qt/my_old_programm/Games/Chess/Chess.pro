#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Chess

FOLDER  = old_programm

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

RESOURCES   += images/chess.qrc

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/chess_mainbox/chess_mainbox.pri)
include (src/new_chessboard/new_chessboard.pri)
#include (src/chessboard/chessboard.pri)

VPATH = $$INCLUDEPATH
