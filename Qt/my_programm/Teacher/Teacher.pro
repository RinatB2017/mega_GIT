#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Teacher

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT += webenginewidgets
QT += network

linux {
    DEFINES += ULTRAWIDE
}

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc \
    image/image.qrc

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)
include (src/notebook/notebook.pri)
include (src/teacher_mainbox/teacher_mainbox.pri)

VPATH = $$INCLUDEPATH
