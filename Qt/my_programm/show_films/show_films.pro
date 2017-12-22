#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = show_films

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui \
    $$PWD/src/treefilms \
    $$PWD/src/basedrag

INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_LOG
DEFINES += NO_TOOLBAR
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp \
    treefilms.hpp

SOURCES += \
    mainbox.cpp \
    treefilms.cpp \
    main.cpp

FORMS += mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

RESOURCES += no_picture.qrc

include (src/selectbox.pri)

LIB_PATH = "../../lib"
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/QXmlPutGet/qxmlputget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

include (resources.pri)

VPATH = $$INCLUDEPATH
