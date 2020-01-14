#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Aggregator

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/aggregator_mainbox \
    $$PWD/src/aggregator_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
} else {
    QT += webkit
}
QT += sql network

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    aggregator_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    aggregator_mainbox.cpp \
    main.cpp

FORMS += aggregator_mainbox.ui

#RESOURCES += icons/temp.qrc

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/database/database.pri)

include ($$LIB_PATH2/web/persistentcookiejar/persistentcookiejar.pri)
include ($$LIB_PATH2/web/websettingbox/websettingbox.pri)
include ($$LIB_PATH2/web/listwidgets/listwidget.pri)
include ($$LIB_PATH2/web/javascript/javascript.pri)
include ($$LIB_PATH2/web/webview/webview.pri)
include ($$LIB_PATH2/web/waiting/waiting.pri)

include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
