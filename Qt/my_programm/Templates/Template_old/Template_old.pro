#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template_old

VER_MAJOR = 1
VER_MINOR = 0
VER_PATCH = 1
VER_BUILD = 0

DEFINES += VER_MAJOR=$${VER_MAJOR}
DEFINES += VER_MINOR=$${VER_MINOR}
DEFINES += VER_BUILD=$${VER_BUILD}
DEFINES += VER_PATCH=$${VER_PATCH}

FOLDER  = _Templates

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    VERSION = $${VER_MAJOR}"."$${VER_MINOR}"."$${VER_PATCH}"."$${VER_BUILD}

    QMAKE_TARGET_COMPANY = Home
    QMAKE_TARGET_PRODUCT = $$TARGET
    QMAKE_TARGET_COPYRIGHT = "Copyright \\251 2020-2025"
    QMAKE_TARGET_DESCRIPTION = "my description"

    RC_ICONS = "ico/computer.ico"
    # RC_FILE  = programm.rc
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    ico/icons.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH = "$$PWD/../../../lib"
include (src/config.pri)    #строка должна быть перед mainwindow
include ($$LIB_PATH/meta/mainwindow.pri)

include (src/template_old_mainbox/template_old_mainbox.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    message($$VERSION_HEADER)
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

#message($$DEFINES)

VPATH = $$INCLUDEPATH
