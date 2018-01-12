#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = WebKit_old

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column
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
    defines.hpp \
    mainbox.hpp

SOURCES += \
    main.cpp \
    mainbox.cpp

FORMS += mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

RESOURCES += \
    pictures/dialog.qrc \
    pictures/places.qrc \
    pictures/devices.qrc \
    pictures/arrow.qrc

RESOURCES += jquery/jquery.qrc

LIB_PATH = "../../../lib"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
