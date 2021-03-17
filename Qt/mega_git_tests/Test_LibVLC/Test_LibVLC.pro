#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_LibVLC

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp \
    version.hpp \

SOURCES += \
    main.cpp

OTHER_FILES += doc/notebook.txt

linux {
    LIBS    += -lvlc
}

win32 {
    include (src/vlc/vlc.pri)

    LIBS    += -L$$PWD/lib
    LIBS    += -lvlc
}

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "$$PWD/../../../Qt/lib"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_LibVLC_mainbox/test_LibVLC_mainbox.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

VPATH = $$INCLUDEPATH
