#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_drawbuttons

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/Test_drawbuttons_mainbox \
    $$PWD/src/Test_drawbuttons_mainbox/ui
INCLUDEPATH = $$DEPENDPATH



#DEFINES += FIXED_SIZE

#DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_LOG

#LIBS    += -lusb

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    Test_drawbuttons_mainbox.hpp

SOURCES += \
    Test_drawbuttons_mainbox.cpp \
    main.cpp

FORMS   += Test_drawbuttons_mainbox.ui

RESOURCES += icons/temp.qrc

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mydisplay.pri)
include ($$LIB_PATH/mywidgets/mybutton.pri)

include ($$LIB_PATH2/csvreader/csvreader.pri)

include ($$LIB_PATH2/CarefulMessageBox/CarefulMessageBox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
