#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = ADNS_2610

FOLDER  = old_programm

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/screen
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp \
    screen.hpp

SOURCES += \
    main.cpp \
    screen.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include (src/adns_2610_mainbox/adns_2610_mainbox.pri)

VPATH = $$INCLUDEPATH
