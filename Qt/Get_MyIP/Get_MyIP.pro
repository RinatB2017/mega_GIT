#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Get_MyIP

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

QT  += network

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_LOG
DEFINES += NO_RESIZE

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS   += mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

LIB_PATH = "../lib"

include ($$LIB_PATH/qtsingleapplication/qtsingleapplication.pri)
include ($$LIB_PATH/mywaitsplashscreen/mywaitsplashscreen.pri)
include ($$LIB_PATH/mysplashscreen/mysplashscreen.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/turbo.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
