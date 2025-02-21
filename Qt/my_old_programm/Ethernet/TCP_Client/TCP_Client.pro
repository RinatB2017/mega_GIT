#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = TCP_Client

FOLDER  = old_programm

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/ethernet/tcp_client/tcp_client.pri)
include ($$LIB_PATH2/widgets/ipv4/ipv4.pri)

#include ($$LIB_PATH/ethernet/fake/fake.pri)

include (src/tcp_client_mainbox/tcp_client_mainbox.pri)

VPATH = $$INCLUDEPATH
