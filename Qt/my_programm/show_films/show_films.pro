#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = show_films

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/show_films_mainbox \
    $$PWD/src/show_films_mainbox/ui \
    $$PWD/src/treefilms \
    $$PWD/src/basedrag

INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_LOG
DEFINES += NO_TOOLBAR
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    show_films_mainbox.hpp \
    treefilms.hpp

SOURCES += \
    show_films_mainbox.cpp \
    treefilms.cpp \
    main.cpp

FORMS   += show_films_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += no_picture.qrc

include (src/selectbox.pri)

LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

include (resources.pri)

VPATH = $$INCLUDEPATH
