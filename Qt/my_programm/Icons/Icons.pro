#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Icons

VER_MAJOR = 1
VER_MINOR = 0
VER_PATCH = 0
VER_BUILD = 0

DEFINES += VER_MAJOR=$${VER_MAJOR}
DEFINES += VER_MINOR=$${VER_MINOR}
DEFINES += VER_BUILD=$${VER_BUILD}
DEFINES += VER_PATCH=$${VER_PATCH}

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

RESOURCES += icons/list_icons.qrc

win32 {
    VERSION = $${VER_MAJOR}"."$${VER_MINOR}"."$${VER_PATCH}"."$${VER_BUILD}

    QMAKE_TARGET_COMPANY = Home
    QMAKE_TARGET_PRODUCT = $$TARGET
    QMAKE_TARGET_COPYRIGHT = "Copyright \\251 2020-2025"
    QMAKE_TARGET_DESCRIPTION = "my description"

    RC_ICONS = "ico/computer.ico"
}

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include (src/config.pri)
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/icons.pri)
include (src/icons_mainbox/icons_mainbox.pri)

QMAKE_RESOURCE_FLAGS += -no-compress

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

VPATH = $$INCLUDEPATH
