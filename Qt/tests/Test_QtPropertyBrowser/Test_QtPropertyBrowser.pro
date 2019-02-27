#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QtPropertyBrowser

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/test_QtPropertyBrowser_mainbox \
    $$PWD/src/test_QtPropertyBrowser_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_QtPropertyBrowser_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_QtPropertyBrowser_mainbox.cpp \
    main.cpp

FORMS   += test_QtPropertyBrowser_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/QtPropertyBrowser/qtpropertybrowser.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
