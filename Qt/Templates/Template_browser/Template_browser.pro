#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template_browser

FOLDER  = _Templates

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
} else {
    QT += webkit
}
QT += network

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS += mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/web/persistentcookiejar/persistentcookiejar.pri)
include ($$LIB_PATH/web/websettingbox/websettingbox.pri)
include ($$LIB_PATH/web/javascript/javascript.pri)
include ($$LIB_PATH/web/webview/webview.pri)
include ($$LIB_PATH/QXmlPutGet/qxmlputget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
