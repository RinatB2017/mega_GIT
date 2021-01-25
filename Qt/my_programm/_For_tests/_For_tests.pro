#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = _For_tests

VER_MAJOR = 1
VER_MINOR = 0
VER_PATCH = 1
VER_BUILD = 0

DEFINES += VER_MAJOR=$${VER_MAJOR}
DEFINES += VER_MINOR=$${VER_MINOR}
DEFINES += VER_BUILD=$${VER_BUILD}
DEFINES += VER_PATCH=$${VER_PATCH}

PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

HEADERS += \
    defines.hpp
    version.hpp

SOURCES += \
    main.cpp

RESOURCES += \
    $$PWD/images/puzzle/puzzle.qrc \
    $$PWD/images/images.qrc \
    $$PWD/images/bullets/bullets.qrc \
    $$PWD/ico/icons.qrc \
    $$PWD/themes/themes_css/themes_css.qrc \
    $$PWD/themes/themes_qss/themes_qss.qrc

#можно поставить новое logo
#RESOURCES += about/about.qrc

OTHER_FILES += \
    doc/notebook.txt

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

CONFIG(debug, debug|release) {
    include (src/test/test.pri)

    # профилирование
    # QMAKE_CXXFLAGS_DEBUG += -pg
    # QMAKE_LFLAGS_DEBUG += -pg
    #---
}

win32 {
    VERSION = $${VER_MAJOR}"."$${VER_MINOR}"."$${VER_PATCH}"."$${VER_BUILD}

    QMAKE_TARGET_COMPANY = Home
    QMAKE_TARGET_PRODUCT = $$TARGET
    QMAKE_TARGET_COPYRIGHT = "Copyright \\251 2020-2025"
    QMAKE_TARGET_DESCRIPTION = "my description"

    RC_ICONS = ico/computer.ico # нужно, чтобы создался фал .rc автоматом
    # RC_FILE  = programm.rc
}

include (src/config.pri)
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/icons/digits.pri)
include ($$LIB_PATH2/icons/arrows.pri)

include ($$LIB_PATH/mywidgets/mylistwidget/mylistwidget.pri)
include ($$LIB_PATH2/widgets/QLongLongSpinBox/QLongLongSpinBox.pri)
#include ($$LIB_PATH2/web/mybrowser/mybrowser.pri)

include (src/for_tests_mainbox/for_tests_mainbox.pri)
include (src/mymainwindow/mymainwindow.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.pri
#    VERSION_HEADER = $$PWD/src/version.hpp
#    message($$VERSION_HEADER)
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

VPATH = $$INCLUDEPATH
