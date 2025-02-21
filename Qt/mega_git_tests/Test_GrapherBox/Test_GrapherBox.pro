#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_GrapherBox

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/computer.ico
}

#----------------------------------------------
win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH2/icons/digits.pri)

include (src/test_GrapherBox_mainbox/test_GrapherBox_mainbox.pri)
include (src/testdialog/testdialog.pri)
#----------------------------------------------

VPATH = $$INCLUDEPATH
