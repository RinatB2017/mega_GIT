#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Get_MyIP

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src 
INCLUDEPATH = $$DEPENDPATH

QT  += network

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

LIB_PATH = "$$PWD/../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/get_myip_mainbox/get_myip_mainbox.pri)

VPATH = $$INCLUDEPATH
