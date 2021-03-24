#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_HLK_RM04

FOLDER  = WiFi

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_HLK_RM04_mainbox \
    $$PWD/src/test_HLK_RM04_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#DEFINES += SAVE_INI

QT      += serialport

HEADERS += \
    test_HLK_RM04_mainbox.hpp \
    defines.hpp

SOURCES += \
    test_HLK_RM04_mainbox.cpp \
    main.cpp

FORMS   += test_HLK_RM04_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate/serialbox5_fix_baudrate.pri)
include ($$LIB_PATH2/serial5/serialbox5_lite/serialbox5_lite.pri)

include ($$LIB_PATH2/QHexEdit_QT5/QHexEdit_QT5.pri)
include ($$LIB_PATH2/wifi_frame/wifi_frame.pri)
include ($$LIB_PATH2/widgets/ipv4/ipv4.pri)

include ($$LIB_PATH2/ethernet/tcp_server/tcp_server.pri)

include (src/hlk_rm04_widget/hlk_rm04_widget.pri)

VPATH = $$INCLUDEPATH
