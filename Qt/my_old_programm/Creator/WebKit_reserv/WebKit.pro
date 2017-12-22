#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   =

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui \
    $$PWD/src/findlinks \
    $$PWD/src/websettingbox \
    $$PWD/src/persistentcookiejar \
    $$PWD/src/listwidgets \
    $$PWD/src/Database
INCLUDEPATH = $$DEPENDPATH

#DEFINES += FLAG_RESIZE
DEFINES += PROGRAMM_IN_UTF8

#DEFINES += MAINWINDOW_LOG
DEFINES += MAINWINDOW_TOOLBAR
#DEFINES += MAINWINDOW_STYLETOOLBAR
DEFINES += NO_TRAYICON

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
} else {
    QT += webkit
}

QT += network sql

HEADERS += \
    defines.hpp \
    mainbox.hpp \
    persistentcookiejar.hpp \
    websettingbox.hpp \
    findlinks.hpp \
    listwidget_site.hpp \
    listwidget_link.hpp \
    database.hpp

SOURCES += \
    main.cpp \
    mainbox.cpp \
    persistentcookiejar.cpp \
    websettingbox.cpp \
    findlinks.cpp \
    listwidget_site.cpp \
    listwidget_link.cpp \
    database.cpp

FORMS += mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

RESOURCES += data/names.qrc

LIB_PATH = "../../lib"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/webview/webview.pri)

VERSION_HEADER = src/version.hpp
include ($$LIB_PATH/auto_inc_version.pri)

VPATH = $$INCLUDEPATH
