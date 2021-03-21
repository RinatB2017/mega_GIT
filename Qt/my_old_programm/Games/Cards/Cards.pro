#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Cards

FOLDER  = old_programm

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT	+= svg
QT      += xml

DEFINES += SAVE_CARD

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
include (src/card/card.pri)

include (src/cards_mainbox/cards_mainbox.pri)

RESOURCES += \
    cards/cards.qrc \
    pictures/pictures.qrc

VPATH = $$INCLUDEPATH
