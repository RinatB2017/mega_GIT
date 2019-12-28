#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Analyzer

FOLDER  = old_programm

DEPENDPATH  += \
    /usr/include/libftdi1 \
    $$PWD/src \
    $$PWD/src/analizer_mainbox \
    $$PWD/src/analizer_mainbox/ui \
    $$PWD/src/ft2232h_thread
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    analizer_mainbox.hpp \
    ft2232h_thread.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    analizer_mainbox.cpp \
    ft2232h_thread.cpp \
    main.cpp

FORMS += analizer_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

unix {
    LIBS    += -lftdi1
}

LIBS    += -lusb
LIBS    += /opt/lib64/libftd2xx.so

LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH/time/time.pri)

win32 {
    include ($$LIB_PATH/ftdi/ftdi.pri)
}

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
