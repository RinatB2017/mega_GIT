#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Chat

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui \
    $$PWD/src/mainbox \
    $$PWD/src/worker
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp \
    mythread.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp \
    mythread.cpp

FORMS += mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../../../lib"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/qtsingleapplication/qtsingleapplication.pri)
include ($$LIB_PATH/mywaitsplashscreen/mywaitsplashscreen.pri)
include ($$LIB_PATH/mysplashscreen/mysplashscreen.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/turbo.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
