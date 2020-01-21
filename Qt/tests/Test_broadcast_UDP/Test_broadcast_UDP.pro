#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_broadcast_UDP

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_broadcast_udp_mainbox \
    $$PWD/src/test_broadcast_udp_mainbox/ui \
    $$PWD/src/worker
INCLUDEPATH = $$DEPENDPATH

QT      += network

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_broadcast_udp_mainbox.hpp \
    defines.hpp \
    version.hpp \
    server.hpp \
    client.hpp

SOURCES += \
    test_broadcast_udp_mainbox.cpp \
    main.cpp \
    server.cpp \
    client.cpp

FORMS   += test_broadcast_udp_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
