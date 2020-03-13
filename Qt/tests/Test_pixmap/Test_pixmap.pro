#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_pixmap

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/test_pixmap_mainbox \
    $$PWD/src/test_pixmap_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_pixmap_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_pixmap_mainbox.cpp \
    main.cpp

FORMS   += test_pixmap_mainbox.ui

RESOURCES   += \
    pictures/pictures.qrc \
    data/data.qrc

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)
include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
