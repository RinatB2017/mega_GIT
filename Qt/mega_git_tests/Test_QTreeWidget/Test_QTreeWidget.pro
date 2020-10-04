#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QTreeWidget

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_QTreeWidget_mainbox/test_QTreeWidget_mainbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
