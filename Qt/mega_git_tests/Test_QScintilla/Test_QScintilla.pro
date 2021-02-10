#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QScintilla

FOLDER  = tests

VER_MAJOR = 0
VER_MINOR = 99
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

unix {
    LIBS    += -lqscintilla2_qt5
}

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

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

    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    data/data.qrc \
    ico/icons.qrc \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

#----------------------------------------------
LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

win32 {
    include ($$LIB_PATH2/QScintilla/QScintilla.pri)
}
include (src/qscintilla_mainbox/qscintilla_mainbox.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    include ($$LIB_PATH/auto_inc_version.pri)
#}
#----------------------------------------------

VPATH = $$INCLUDEPATH
