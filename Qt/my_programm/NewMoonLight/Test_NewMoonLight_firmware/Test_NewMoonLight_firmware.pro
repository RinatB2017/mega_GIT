#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Test_NewMoonLight_firmware

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_newmoonlight_firmware_mainbox \
    $$PWD/src/test_newmoonlight_firmware_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON
#DEFINES += FAKE
#DEFINES += NO_LOG
#DEFINES += MODBUS

DEFINES += SAVE_WIDGETS_SPINBOX

HEADERS += \
    test_newmoonlight_firmware_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_newmoonlight_firmware_mainbox.cpp \
    main.cpp

FORMS   += test_newmoonlight_firmware_mainbox.ui

RESOURCES   += \
    pic/pic.qrc \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include (src/button/button.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
