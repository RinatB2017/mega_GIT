#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = WebKit

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += FLAG_RESIZE
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_LOG
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
} else {
    QT += webkit
}

QT += network testlib

HEADERS += \
    defines.hpp \
    mainbox.hpp

SOURCES += \
    main.cpp \
    mainbox.cpp

FORMS += mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

RESOURCES += data/names.qrc

LIB_PATH = "../../../lib"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/database/database.pri)

include ($$LIB_PATH/web/persistentcookiejar/persistentcookiejar.pri)
include ($$LIB_PATH/web/websettingbox/websettingbox.pri)
include ($$LIB_PATH/web/listwidgets/listwidget.pri)
include ($$LIB_PATH/web/javascript/javascript.pri)
include ($$LIB_PATH/web/tableview/tableview.pri)
include ($$LIB_PATH/web/findlinks/findlinks.pri)
include ($$LIB_PATH/QXmlPutGet/qxmlputget.pri)
include ($$LIB_PATH/web/waiting/waiting.pri)
include ($$LIB_PATH/web/webview/webview.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
