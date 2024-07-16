#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_thread

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_thread_mainbox \
    $$PWD/src/test_thread_mainbox/ui \
    $$PWD/src/worker
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    test_thread_mainbox.hpp \
    defines.hpp \
    mythread.hpp

SOURCES += \
    test_thread_mainbox.cpp \
    main.cpp \
    mythread.cpp

FORMS   += test_thread_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

VPATH = $$INCLUDEPATH
