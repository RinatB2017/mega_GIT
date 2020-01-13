#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = ProxyWalker_M

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/proxy_walker_m_mainbox \
    $$PWD/src/proxy_walker_m_mainbox/ui \
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

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += SAVE_WIDGETS_SPLITTER

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    proxy_walker_m_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    proxy_walker_m_mainbox.cpp \
    main.cpp

FORMS   += proxy_walker_m_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)

include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)

include ($$LIB_PATH2/web/persistentcookiejar/persistentcookiejar.pri)
include ($$LIB_PATH2/web/websettingbox/websettingbox.pri)
include ($$LIB_PATH2/web/javascript/javascript.pri)
include ($$LIB_PATH2/web/webview/webview.pri)
include ($$LIB_PATH2/web/waiting/waiting.pri)

include ($$LIB_PATH/turbo.pri)

include (src/mymainwindow/mymainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
