#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Get_MyIP

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/get_myip_mainbox \
    $$PWD/src/get_myip_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += network

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_LOG
DEFINES += NO_RESIZE

HEADERS += \
    get_myip_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    get_myip_mainbox.cpp \
    main.cpp

FORMS   += get_myip_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

LIB_PATH = "../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
