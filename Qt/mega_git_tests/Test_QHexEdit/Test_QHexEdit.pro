#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QHexEdit

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

win32 {
    RC_ICONS = ico/computer.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/QHexEdit_QT5/QHexEdit_QT5.pri)

include (src/test_QHexEdit_mainbox/test_QHexEdit_mainbox.pri)

VPATH = $$INCLUDEPATH
