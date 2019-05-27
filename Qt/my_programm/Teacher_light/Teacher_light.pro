#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Teacher_light

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/teacher_light_mainbox \
    $$PWD/src/teacher_light_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

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

DEFINES += NO_RESIZE

DEFINES += USE_DOCKS

HEADERS += \
    teacher_light_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    teacher_light_mainbox.cpp \
    main.cpp

FORMS   += teacher_light_mainbox.ui

OTHER_FILES += doc/notebook.txt

RESOURCES   += image/image.qrc

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += images/images.qrc

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
