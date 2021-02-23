#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Teacher

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

QT += webenginewidgets
QT += network

linux {
    DEFINES += ULTRAWIDE
}

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc \
    image/image.qrc

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include (src/config.pri)
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)
include (src/notebook/notebook.pri)
include (src/teacher_mainbox/teacher_mainbox.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

VPATH = $$INCLUDEPATH
