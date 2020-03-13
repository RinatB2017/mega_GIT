#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Teacher_reserv

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/teacher_mainbox \
    $$PWD/src/teacher_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
} else {
    QT += webkit
}
QT += network

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_LOG

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    teacher_mainbox.hpp \
    defines.hpp \
    version.hpp \

SOURCES += \
    teacher_mainbox.cpp \
    main.cpp

FORMS   += teacher_mainbox.ui

OTHER_FILES += doc/notebook.txt

RESOURCES   += image/image.qrc

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "$$PWD/../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
