#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_WebKit

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_WebKit_mainbox \
    $$PWD/src/test_WebKit_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
} else {
    QT += webkit
}

QT += network

HEADERS += \
    test_WebKit_mainbox.hpp \
    defines.hpp \

SOURCES += \
    test_WebKit_mainbox.cpp \
    main.cpp

FORMS   += test_WebKit_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/database/database.pri)

include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)

include ($$LIB_PATH2/web/persistentcookiejar/persistentcookiejar.pri)
include ($$LIB_PATH2/web/websettingbox/websettingbox.pri)
include ($$LIB_PATH2/web/listwidgets/listwidget.pri)
include ($$LIB_PATH2/web/javascript/javascript.pri)
include ($$LIB_PATH2/web/tableview/tableview.pri)
include ($$LIB_PATH2/web/findlinks/findlinks.pri)
include ($$LIB_PATH2/web/waiting/waiting.pri)
include ($$LIB_PATH2/web/webview/webview.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

VPATH = $$INCLUDEPATH
