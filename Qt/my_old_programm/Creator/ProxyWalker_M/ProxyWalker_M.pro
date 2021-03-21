#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = ProxyWalker_M

FOLDER  = old_programm

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/waiting \
    $$PWD/src/tester
INCLUDEPATH = $$DEPENDPATH

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
} else {
    QT += webkit
}
QT += network

QT += testlib network webkit

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)

include ($$LIB_PATH2/web/persistentcookiejar/persistentcookiejar.pri)
include ($$LIB_PATH2/web/websettingbox/websettingbox.pri)
include ($$LIB_PATH2/web/javascript/javascript.pri)
include ($$LIB_PATH2/web/webview/webview.pri)
include ($$LIB_PATH2/web/waiting/waiting.pri)

include (src/mymainwindow/mymainwindow.pri)

include (src/proxy_walker_m_mainbox/proxy_walker_m_mainbox.pri)

VPATH = $$INCLUDEPATH
