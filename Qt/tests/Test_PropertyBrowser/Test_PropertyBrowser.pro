#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = TestPropertyBrowser

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/Test_PropertyBrowser_mainbox \
    $$PWD/src/Test_PropertyBrowser_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column


DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    Test_PropertyBrowser_mainbox.hpp

SOURCES += \
    Test_PropertyBrowser_mainbox.cpp \
    main.cpp

FORMS   += Test_PropertyBrowser_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

RESOURCES += demo.qrc

LIB_PATH = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/QtPropertyBrowser/qtpropertybrowser.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
