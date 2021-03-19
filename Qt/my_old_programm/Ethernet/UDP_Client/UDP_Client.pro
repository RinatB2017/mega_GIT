#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = UDP_Client

FOLDER  = old_programm

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/ethernet/udp_client/udp_client.pri)
include ($$LIB_PATH2/widgets/ipv4/ipv4.pri)

#include ($$LIB_PATH/ethernet/fake/fake.pri)

include (src/udp_client_mainbox/udp_client_mainbox.pri)

VPATH = $$INCLUDEPATH
