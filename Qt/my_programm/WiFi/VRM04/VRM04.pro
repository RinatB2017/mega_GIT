#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = VRM04

FOLDER  = WiFi

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/VRM04_mainbox \
    $$PWD/src/VRM04_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

HEADERS += \
    VRM04_mainbox.hpp \
    defines.hpp

SOURCES += \
    VRM04_mainbox.cpp \
    main.cpp

FORMS   += VRM04_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include ($$LIB_PATH/time/time.pri)

include ($$LIB_PATH2/QHexEdit_QT5/QHexEdit_QT5.pri)

include ($$LIB_PATH2/wifi_frame/wifi_frame.pri)

VPATH = $$INCLUDEPATH
