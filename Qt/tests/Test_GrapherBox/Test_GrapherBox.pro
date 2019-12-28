#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_GrapherBox

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/test_GrapherBox_mainbox \
    $$PWD/src/test_GrapherBox_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

#DEFINES += CONTEXT_MENU

#DEFINES += USE_SCALE_POINT_DATETIME
#DEFINES += USE_SCALE_POINT_TIME

DEFINES += PROGRAMM_IN_UTF8

#DEFINES += ONE_CURVE

HEADERS += \
    test_GrapherBox_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_GrapherBox_mainbox.cpp \
    main.cpp

FORMS   += test_GrapherBox_mainbox.ui

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/computer.ico
}

#----------------------------------------------
LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH2/icons/digits.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
