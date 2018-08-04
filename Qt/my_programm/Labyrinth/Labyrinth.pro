#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Labyrinth

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/labyrinth_mainbox \
    $$PWD/src/labyrinth_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

DEFINES += SAVE_WIDGETS_SPINBOX

HEADERS += \
    defines.hpp \
    version.hpp \
    labyrinth_mainbox.hpp

SOURCES += \
    labyrinth_mainbox.cpp \
    main.cpp

FORMS   += labyrinth_mainbox.ui

RESOURCES   += \
    images/buttons.qrc \
    test_map/test_map.qrc

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)
include ($$LIB_PATH/meta/mainwindow.pri)

include (src/minimap/minimap.pri)
include (src/map/map.pri)
include (src/mouse/mouse.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
