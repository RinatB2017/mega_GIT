#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_GrapherBox

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/Test_GrapherBox_mainbox \
    $$PWD/src/Test_GrapherBox_mainbox/ui \
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

#DEFINES += CONTEXT_MENU

#DEFINES += USE_SCALE_POINT_DATETIME
#DEFINES += USE_SCALE_POINT_TIME

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    Test_GrapherBox_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    Test_GrapherBox_mainbox.cpp \
    main.cpp

FORMS   += Test_GrapherBox_mainbox.ui

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_FILE = ico/myapp.rc
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
