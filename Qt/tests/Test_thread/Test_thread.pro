#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_thread

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/Test_thread_mainbox \
    $$PWD/src/Test_thread_mainbox/ui \
    $$PWD/src/worker
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_LOG

HEADERS += \
    defines.hpp \
    version.hpp \
    Test_thread_mainbox.hpp \
    mythread.hpp

SOURCES += \
    Test_thread_mainbox.cpp \
    main.cpp \
    mythread.cpp

FORMS   += Test_thread_mainbox.ui

OTHER_FILES += doc/notebook.txt

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
