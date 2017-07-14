#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Teacher

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
} else {
    QT += webkit
}
QT += network

QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_LOG

#DEFINES += SAVE_WIDGETS_CHECKBOX
#DEFINES += SAVE_WIDGETS_COMBOBOX
#DEFINES += SAVE_WIDGETS_PUSHBUTTON
#DEFINES += SAVE_WIDGETS_TOOLBUTTON
#DEFINES += SAVE_WIDGETS_SPINBOX
#DEFINES += SAVE_WIDGETS_DOUBLESPINBOX
#DEFINES += SAVE_WIDGETS_SLIDER
DEFINES += SAVE_WIDGETS_SPLITTER
DEFINES += SAVE_WIDGETS_TEXTEDIT
DEFINES += SAVE_WIDGETS_LINEEDIT

DEFINES += PROGRAMM_IN_UTF8


DEFINES += ULTRAWIDE

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS   += mainbox.ui

OTHER_FILES += doc/notebook.txt

RESOURCES   += image/image.qrc

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../lib"
include ($$LIB_PATH/meta/mainwindow.pri)
include (src/notebook/notebook.pri)
include ($$LIB_PATH/QXmlPutGet/qxmlputget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
