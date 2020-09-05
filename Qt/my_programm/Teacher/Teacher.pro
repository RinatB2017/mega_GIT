#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Teacher

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/teacher_mainbox \
    $$PWD/src/teacher_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT += webenginewidgets
QT += network

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_LOG

DEFINES += PROGRAMM_IN_UTF8

linux {
    DEFINES += ULTRAWIDE
}

HEADERS += \
    teacher_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    teacher_mainbox.cpp \
    main.cpp

FORMS   += teacher_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc \
    image/image.qrc

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include (src/notebook/notebook.pri)
include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
