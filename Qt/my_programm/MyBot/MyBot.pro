#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = MyBot

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

unix {
    HEADERS += other.hpp
}

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc \
    cards/cards.qrc \
    xml/xml.qrc 

OTHER_FILES += doc/notebook.txt

CONFIG  += c++11

unix {
    LIBS    += -lX11 -lXtst
#    LIBS    += -lOpenCL
}

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/WebCamera/WebCamera.pri)
include (src/mybot_mainbox/mybot_mainbox.pri)

VPATH = $$INCLUDEPATH
