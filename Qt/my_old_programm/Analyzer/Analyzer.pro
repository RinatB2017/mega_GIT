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
    defines.hpp

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

LIBS    += -lusb-1.0
#LIBS    += /opt/lib64/libftd2xx.so
LIBS    += /usr/lib64/libftd2xx.so

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH/time/time.pri)

include (src/analizer_mainbox/analizer_mainbox.pri)

win32 {
    include ($$LIB_PATH/ftdi/ftdi.pri)
}

VPATH = $$INCLUDEPATH
