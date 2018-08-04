#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_md5

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/Test_md5_mainbox \
    $$PWD/src/Test_md5_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    Test_md5_mainbox.hpp

SOURCES += \
    Test_md5_mainbox.cpp \
    main.cpp

FORMS   += Test_md5_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../Qt/lib"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
