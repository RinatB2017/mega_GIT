#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_curvetracker

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/Test_curvetracker_mainbox \
    $$PWD/src/Test_curvetracker_mainbox/ui \
    $$PWD/src/curvetracker \
    $$PWD/src/plot
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE

#
#DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_MENU
#DEFINES += NO_LOG

#DEFINES += LOG_READ_ONLY

#DEFINES += SAVE_WIDGETS_PROPERTIES
#DEFINES += SAVE_WIDGETS_CHECKBOX
#DEFINES += SAVE_WIDGETS_COMBOBOX
#DEFINES += SAVE_WIDGETS_PUSHBUTTON
#DEFINES += SAVE_WIDGETS_TOOLBUTTON
#DEFINES += SAVE_WIDGETS_SPINBOX
#DEFINES += SAVE_WIDGETS_DOUBLESPINBOX
#DEFINES += SAVE_WIDGETS_SLIDER
#DEFINES += SAVE_WIDGETS_TEXTEDIT

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    Test_curvetracker_mainbox.hpp

SOURCES += \
    Test_curvetracker_mainbox.cpp \
    main.cpp

FORMS   += Test_curvetracker_mainbox.ui

HEADERS	+= curvetracker.h
SOURCES	+= curvetracker.cpp

#HEADERS    += plot.h
#SOURCES    += plot.cpp

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}


LIB_PATH = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/qwt/qwt.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
