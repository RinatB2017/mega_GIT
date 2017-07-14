#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Navi_server

FOLDER  = organizations/Selena

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/server
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

HEADERS += \
    server.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    server.cpp \
    main.cpp

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../../lib"

include ($$LIB_PATH/proto_NMEA_0183/proto_NMEA_0183.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/ethernet/udp_server.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/terminal.pri)
include ($$LIB_PATH/turbo.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
