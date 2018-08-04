#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QListWidget

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/Test_QListWidget_mainbox \
    $$PWD/src/Test_QListWidget_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_LOG

DEFINES += PROGRAMM_IN_UTF8


HEADERS += \
    defines.hpp \
    version.hpp \
    Test_QListWidget_mainbox.hpp

SOURCES += \
    Test_QListWidget_mainbox.cpp \
    main.cpp

FORMS   += Test_QListWidget_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

include (src/test/test.pri)


LIB_PATH = "../../../Qt/lib"
include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
