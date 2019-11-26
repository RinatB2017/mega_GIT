#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_HTU21D

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_HTU21D_mainbox \
    $$PWD/src/test_HTU21D_mainbox/ui \
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += SHOW_SIZE
#DEFINES += NO_MENU

DEFINES += PROGRAMM_IN_UTF8
#DEFINES += NO_LOG

HEADERS += \
    test_HTU21D_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_HTU21D_mainbox.cpp \
    main.cpp

FORMS   += test_HTU21D_mainbox.ui

OTHER_FILES += \
    doc/notebook.txt \
    arduino/HTU21D_Demo/HTU21D_Demo.ino

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/I2c_logo.ico
}

RESOURCES += \
    ico/icons.qrc

#----------------------------------------------
LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
#include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate_win7/serialbox5_fix_baudrate_win7.pri)
include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate/serialbox5_fix_baudrate.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
