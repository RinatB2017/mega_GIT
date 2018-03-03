#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = ProxyWalker

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/proxy_walker_mainbox \
    $$PWD/src/proxy_walker_mainbox/ui \
    $$PWD/src/waiting \
    $$PWD/src/tester
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

QT += testlib network webkit webkitwidgets

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    proxy_walker_mainbox.hpp

SOURCES += \
    proxy_walker_mainbox.cpp \
    main.cpp

FORMS   += proxy_walker_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/turbo.pri)

include ($$LIB_PATH2/web/webview/webview.pri)
include ($$LIB_PATH2/web/websettingbox/websettingbox.pri)
include ($$LIB_PATH2/web/javascript/javascript.pri)
include ($$LIB_PATH2/web/waiting/waiting.pri)

VERSION_HEADER = src/version.hpp
include ($$LIB_PATH/auto_inc_version.pri)

VPATH = $$INCLUDEPATH
