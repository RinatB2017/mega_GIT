#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_drawbuttons

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

#LIBS    += -lusb

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

RESOURCES += icons/temp.qrc

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mydisplay/mydisplay.pri)
include ($$LIB_PATH/mywidgets/mybutton/mybutton.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH2/CarefulMessageBox/CarefulMessageBox.pri)

include (src/test_drawbuttons_mainbox/test_drawbuttons_mainbox.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

VPATH = $$INCLUDEPATH
