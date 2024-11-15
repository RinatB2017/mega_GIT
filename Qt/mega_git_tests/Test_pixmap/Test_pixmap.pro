#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_pixmap

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

RESOURCES   += \
    pictures/pictures.qrc \
    data/data.qrc

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH2/QXmlPutGet/QXmlPutGet.pri)
include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_pixmap_mainbox/test_pixmap_mainbox.pri)

VPATH = $$INCLUDEPATH
