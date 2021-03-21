#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QtPropertyBrowser

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_QtPropertyBrowser_mainbox \
    $$PWD/src/test_QtPropertyBrowser_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    test_QtPropertyBrowser_mainbox.hpp \
    defines.hpp

SOURCES += \
    test_QtPropertyBrowser_mainbox.cpp \
    main.cpp

FORMS   += test_QtPropertyBrowser_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/QtPropertyBrowser/qtpropertybrowser.pri)

VPATH = $$INCLUDEPATH
