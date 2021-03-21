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

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)
include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_pixmap_mainbox/test_pixmap_mainbox.pri)

VPATH = $$INCLUDEPATH
