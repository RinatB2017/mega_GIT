#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Teacher

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/teacher_mainbox \
    $$PWD/src/teacher_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT += webenginewidgets
QT += network

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_LOG

DEFINES += SAVE_WIDGETS_SPLITTER
DEFINES += SAVE_WIDGETS_TEXTEDIT
DEFINES += SAVE_WIDGETS_LINEEDIT

DEFINES += PROGRAMM_IN_UTF8

DEFINES += ULTRAWIDE

HEADERS += \
    defines.hpp \
    version.hpp \
    teacher_mainbox.hpp

SOURCES += \
    teacher_mainbox.cpp \
    main.cpp

FORMS   += teacher_mainbox.ui

OTHER_FILES += doc/notebook.txt

RESOURCES   += image/image.qrc

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include (src/notebook/notebook.pri)
include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
