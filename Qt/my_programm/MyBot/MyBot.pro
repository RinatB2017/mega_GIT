#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = MyBot

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/mybot_mainbox \
    $$PWD/src/mybot_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    mybot_mainbox.hpp

unix {
    HEADERS += other.hpp
}

SOURCES += \
    mybot_mainbox.cpp \
    main.cpp

FORMS   += mybot_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

CONFIG  += c++11

unix {
    LIBS    += -lX11 -lXtst
}
LIBS    += -lOpenCL

LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/WebCamera/WebCamera.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
