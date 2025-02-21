#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QTreeWidget

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_QTreeWidget_mainbox/test_QTreeWidget_mainbox.pri)

VPATH = $$INCLUDEPATH
