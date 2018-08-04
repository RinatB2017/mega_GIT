#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Template_old

FOLDER  = _Templates

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/template_old_mainbox \
    $$PWD/src/template_old_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

#QT  += serialbus

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE
#
#DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
DEFINES += NO_RESIZE
#DEFINES += NO_MENU
#DEFINES += NO_LOG

#DEFINES += ONLY_ENGLISH

#DEFINES += LOG_READ_ONLY

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
    defines.hpp \
    version.hpp \
    template_old_mainbox.hpp

SOURCES += \
    template_old_mainbox.cpp \
    main.cpp

FORMS   += template_old_mainbox.ui
#FORMS   += template_old_mainbox_test.ui

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
LIB_PATH = "../../../lib"
include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
