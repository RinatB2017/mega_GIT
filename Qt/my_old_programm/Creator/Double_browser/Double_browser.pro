#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Double_browser

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/double_browser_mainbox \
    $$PWD/src/double_browser_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
} else {
    QT += webkit
}
QT += network

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_TOOLBAR
DEFINES += NO_LOG

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    double_browser_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    double_browser_mainbox.cpp \
    main.cpp

FORMS += double_browser_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)

include ($$LIB_PATH2/web/persistentcookiejar/persistentcookiejar.pri)
include ($$LIB_PATH2/web/websettingbox/websettingbox.pri)
include ($$LIB_PATH2/web/javascript/javascript.pri)
include ($$LIB_PATH2/web/webview/webview.pri)

include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)

DESTDIR = bin

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
