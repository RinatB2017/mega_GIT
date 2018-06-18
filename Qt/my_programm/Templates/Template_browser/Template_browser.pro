#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template_browser

FOLDER  = _Templates

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/template_browser_mainbox \
    $$PWD/src/template_browser_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column
QT += webkit webkitwidgets network
#QT  += webenginewidgets

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

DEFINES += BLOCK_WHEEL

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    posix.hpp \
    template_browser_mainbox.hpp

SOURCES += \
    template_browser_mainbox.cpp \
    main.cpp

FORMS   += template_browser_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)

include ($$LIB_PATH2/web/persistentcookiejar/persistentcookiejar.pri)
include ($$LIB_PATH2/web/websettingbox/websettingbox.pri)
include ($$LIB_PATH2/web/javascript/javascript.pri)
include ($$LIB_PATH2/web/webview/webview.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
