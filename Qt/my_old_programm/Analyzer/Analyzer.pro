#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Analyzer

FOLDER  = old_programm

include (conf/conf.pri)

DEPENDPATH  += \
    /usr/include/libftdi1 \
    $$PWD/src \
    $$PWD/src/ft2232h_thread
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    ft2232h_thread.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    ft2232h_thread.cpp \
    main.cpp

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

unix {
    LIBS    += -lftdi1
}

LIBS    += -lusb
LIBS    += /opt/lib64/libftd2xx.so

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH/time/time.pri)

include (src/analizer_mainbox/analizer_mainbox.pri)

win32 {
    include ($$LIB_PATH/ftdi/ftdi.pri)
}

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    include ($$LIB_PATH/auto_inc_version.pri)
#}

VPATH = $$INCLUDEPATH
