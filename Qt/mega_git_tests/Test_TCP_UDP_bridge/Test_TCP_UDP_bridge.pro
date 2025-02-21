#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_TCP_UDP_bridge

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_tcp_udp_thread_mainbox \
    $$PWD/src/test_tcp_udp_thread_mainbox/ui \
    $$PWD/src/worker
INCLUDEPATH = $$DEPENDPATH

QT  += network

HEADERS += \
    test_tcp_udp_thread_mainbox.hpp \
    defines.hpp \
    mythread.hpp

SOURCES += \
    test_tcp_udp_thread_mainbox.cpp \
    mythread.cpp \
    main.cpp

FORMS   += test_tcp_udp_thread_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)

VPATH = $$INCLUDEPATH
