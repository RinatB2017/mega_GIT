#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Labirint

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8


DEFINES += SAVE_WIDGETS_SPINBOX

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS   += mainbox.ui

RESOURCES   += \
    images/buttons.qrc \
    map/map.qrc

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"

include ($$LIB_PATH/QXmlPutGet/qxmlputget.pri)
include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
