#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_RSA

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/test_rsa_mainbox \
    $$PWD/src/test_rsa_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE
#
DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
DEFINES += SHOW_SIZE
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

CONFIG += crypto
LIBS += -lqca-qt5

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_rsa_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_rsa_mainbox.cpp \
    main.cpp

FORMS   += test_rsa_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += \
    doc/notebook.txt \
    orig/rsatest.cpp

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH = "../../../Qt/lib"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
