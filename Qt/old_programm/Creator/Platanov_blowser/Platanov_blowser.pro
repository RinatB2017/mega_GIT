#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Platanov_blowser

FOLDER  = old_programm

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
    src/defines.hpp \
    src/version.hpp \
    src/mainbox/mainbox.hpp

SOURCES += \
    src/mainbox/mainbox.cpp \
    src/main.cpp

FORMS += mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"

include ($$LIB_PATH/web/persistentcookiejar/persistentcookiejar.pri)
include ($$LIB_PATH/web/websettingbox/websettingbox.pri)
include ($$LIB_PATH/web/javascript/javascript.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/QXmlPutGet/qxmlputget.pri)
include ($$LIB_PATH/web/waiting/waiting.pri)
include ($$LIB_PATH/web/webview/webview.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
