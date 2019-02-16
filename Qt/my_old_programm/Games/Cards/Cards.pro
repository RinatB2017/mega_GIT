#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Cards

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/cards_mainbox \
    $$PWD/src/cards_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT	+= svg
QT      += xml

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

DEFINES += SAVE_CARD

HEADERS += \
    cards_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    cards_mainbox.cpp \
    main.cpp

FORMS   += cards_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include (src/card/card.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH

RESOURCES += \
    pictures/pictures.qrc
