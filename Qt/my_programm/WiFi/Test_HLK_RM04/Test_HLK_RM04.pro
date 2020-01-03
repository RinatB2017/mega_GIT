#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_HLK_RM04

FOLDER  = WiFi

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_HLK_RM04_mainbox \
    $$PWD/src/test_HLK_RM04_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8
#DEFINES += RS232_SEND

DEFINES += SAVE_WIDGETS_COMBOBOX
DEFINES += SAVE_WIDGETS_LINEEDIT
DEFINES += SAVE_WIDGETS_SPINBOX
#DEFINES += NO_LOG

#DEFINES += SAVE_INI

QT      += serialport

HEADERS += \
    test_HLK_RM04_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_HLK_RM04_mainbox.cpp \
    main.cpp

FORMS   += test_HLK_RM04_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "../../../lib"
LIB_PATH2 = "../../../lib2"

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)

#include ($$LIB_PATH2/serial5/serial5.pri)
include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate/serialbox5_fix_baudrate.pri)
include ($$LIB_PATH2/serial5/serialbox5_lite/serialbox5_lite.pri)
include ($$LIB_PATH2/serial5/serialbox5/serialbox5.pri)
include ($$LIB_PATH2/serial5/sendbox5/sendbox5.pri)

include ($$LIB_PATH2/QHexEdit_QT5/QHexEdit_QT5.pri)
include ($$LIB_PATH2/wifi_frame/wifi_frame.pri)
include ($$LIB_PATH2/widgets/ipv4/ipv4.pri)

include ($$LIB_PATH2/ethernet/tcp_server.pri)

include (src/hlk_rm04_widget/hlk_rm04_widget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
