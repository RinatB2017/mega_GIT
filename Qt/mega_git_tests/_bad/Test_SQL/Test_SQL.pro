#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_SQL

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT  += sql

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/OpenRPT/OpenRPT.pri)
include (src/test_SQL_mainbox/test_SQL_mainbox.pri)

VPATH = $$INCLUDEPATH
