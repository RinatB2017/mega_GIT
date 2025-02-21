#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Grapher

FOLDER  = old_programm

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/GrapherBox \
    $$PWD/src/GrapherBox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232
#DEFINES += RS232_LOG

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

VPATH = $$INCLUDEPATH
