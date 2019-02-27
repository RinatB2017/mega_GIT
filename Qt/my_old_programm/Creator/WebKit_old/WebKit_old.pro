#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = WebKit_old

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/webkit_mainbox \
    $$PWD/src/webkit_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -std=c++11

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

DEFINES += NO_STYLETOOLBAR

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
} else {
    QT += webkit
}

QT += network sql

HEADERS += \
    webkit_mainbox.hpp \
    defines.hpp

SOURCES += \
    webkit_mainbox.cpp \
    main.cpp

FORMS   += webkit_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

RESOURCES += jquery/jquery.qrc

LIB_PATH  = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)

include ($$LIB_PATH2/pictures/pictures.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
