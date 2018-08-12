#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_mesh

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/mesh_mainbox \
    $$PWD/src/mesh_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_RESIZE
DEFINES += NO_LOG

#DEFINES += SAVE_WIDGETS_CHECKBOX
#DEFINES += SAVE_WIDGETS_COMBOBOX
#DEFINES += SAVE_WIDGETS_PUSHBUTTON
#DEFINES += SAVE_WIDGETS_TOOLBUTTON
#DEFINES += SAVE_WIDGETS_SPINBOX
#DEFINES += SAVE_WIDGETS_DOUBLESPINBOX
#DEFINES += SAVE_WIDGETS_SLIDER
#DEFINES += SAVE_WIDGETS_TEXTEDIT
#DEFINES += SAVE_WIDGETS_LINEEDIT
#DEFINES += SAVE_WIDGETS_SPLITTER

#DEFINES += NO_TOOLBAR_BUTTON_EXIT
#DEFINES += NO_TOOLBAR_BUTTON_FONT
#DEFINES += NO_TOOLBAR_BUTTON_LANG
#DEFINES += NO_TOOLBAR_BUTTON_STYLE
#DEFINES += NO_TOOLBAR_BUTTON_ABOUT
#DEFINES += NO_TOOLBAR_BUTTON_HELP
#DEFINES += NO_TOOLBAR_SEPARATORS

#DEFINES += DEMO
#DEFINES += "DEMO_YEAR=2017"
#DEFINES += "DEMO_MONTH=9"
#DEFINES += "DEMO_DAY=9"

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    mesh_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    mesh_mainbox.cpp \
    main.cpp

FORMS   += mesh_mainbox.ui

OTHER_FILES += arduino/Mesh_unit/Mesh_unit.ino
OTHER_FILES += arduino/basic/basic.ino

win32 {
    RC_FILE = ico/myapp.rc
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

#----------------------------------------------
LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include (src/mesh_control/mesh_control.pri)
include (src/mesh_led/mesh_led.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
