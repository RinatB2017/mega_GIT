#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Teacher_light

include (conf/conf.pri)

PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES   += \
    images/images.qrc \
    image/image.qrc

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/QXmlPutGet/qxmlputget.pri)
include (src/teacher_light_mainbox/teacher_light_mainbox.pri)
include (src/notebook/notebook.pri)

VPATH = $$INCLUDEPATH
