#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Icons

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

RESOURCES += icons/list_icons.qrc

win32 {
    RC_ICONS = "ico/computer.ico"
}

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/icons.pri)
include (src/icons_mainbox/icons_mainbox.pri)

QMAKE_RESOURCE_FLAGS += -no-compress

VPATH = $$INCLUDEPATH
