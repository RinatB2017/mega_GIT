#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QListWidget

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

include (src/test/test.pri)

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include (src/test_QListWidget_mainbox/test_QListWidget_mainbox.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

VPATH = $$INCLUDEPATH
