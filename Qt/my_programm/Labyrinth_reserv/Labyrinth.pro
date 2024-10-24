#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Labyrinth

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/labyrinth_mainbox \
    $$PWD/src/labyrinth_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    labyrinth_mainbox.hpp \
    defines.hpp \
    version.hpp

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

LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)
include ($$LIB_PATH/meta/mainwindow.pri)

include (src/minimap/minimap.pri)
include (src/map/map.pri)
include (src/mouse/mouse.pri)

# !exists(OBJECTS_DIR) {
#     VERSION_HEADER = src/version.hpp
#     include ($$LIB_PATH/auto_inc_version.pri)
# }

VPATH = $$INCLUDEPATH
