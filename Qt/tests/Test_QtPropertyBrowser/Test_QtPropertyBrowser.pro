#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QtPropertyBrowser

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/Test_QtPropertyBrowser_mainbox \
    $$PWD/src/Test_QtPropertyBrowser_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    Test_QtPropertyBrowser_mainbox.hpp

SOURCES += \
    Test_QtPropertyBrowser_mainbox.cpp \
    main.cpp

FORMS   += Test_QtPropertyBrowser_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/QtPropertyBrowser/qtpropertybrowser.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
