#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = WebKit

FOLDER  = old_programm

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
} else {
    QT += webkit
}

QT += network testlib

HEADERS += \
    defines.hpp 

SOURCES += \
    main.cpp 

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += data/names.qrc

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

CONFIG(debug, debug|release) {
    QT  += testlib
}

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

include (src/webkit_mainbox/webkit_mainbox.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

VPATH = $$INCLUDEPATH
