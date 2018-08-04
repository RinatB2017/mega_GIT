#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Test_NewMoonLight

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_newmoonlight_mainbox \
    $$PWD/src/test_newmoonlight_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += RS232_FIXED_SIZE
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
    defines.hpp \
    version.hpp \
    test_newmoonlight_mainbox.hpp

SOURCES += \
    test_newmoonlight_mainbox.cpp \
    main.cpp

FORMS   += test_newmoonlight_mainbox.ui

RESOURCES   += pic/pic.qrc

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include (src/led/led.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
