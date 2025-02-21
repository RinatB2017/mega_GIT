#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = RS232_5_x2

include (conf/conf.pri)

FOLDER      = RS232

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

#DEFINES += RS232_FIXED_SIZE
DEFINES += RS232_SEND
DEFINES += RS232_LOG

SOURCES += main.cpp

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/RS232.ico
}

win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include (src/test_widget/test_widget.pri)

RESOURCES += \
    ico/icons.qrc

VPATH = $$INCLUDEPATH
