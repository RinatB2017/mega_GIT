#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = show_films

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/show_films_mainbox \
    $$PWD/src/show_films_mainbox/ui \
    $$PWD/src/treefilms \
    $$PWD/src/basedrag

INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp \
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

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)

include (resources.pri)

VPATH = $$INCLUDEPATH
