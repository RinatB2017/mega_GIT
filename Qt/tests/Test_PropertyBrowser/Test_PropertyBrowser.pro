#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = TestPropertyBrowser

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_PropertyBrowser_mainbox \
    $$PWD/src/test_PropertyBrowser_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

HEADERS += \
    test_PropertyBrowser_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_PropertyBrowser_mainbox.cpp \
    main.cpp

FORMS   += test_PropertyBrowser_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += demo.qrc

LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/QtPropertyBrowser/qtpropertybrowser.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
