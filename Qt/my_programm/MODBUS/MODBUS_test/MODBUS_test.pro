#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = MODBUS_test

FOLDER  = modbus

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/modbus_test_mainbox \
    $$PWD/src/modbus_test_mainbox/ui 
INCLUDEPATH = $$DEPENDPATH

QT  += \
    serialport \
    serialbus \
    network

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += SAVE_WIDGETS_SPINBOX

DEFINES += PROGRAMM_IN_UTF8

DEFINES += HEX16

HEADERS += \
    modbus_test_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    modbus_test_mainbox.cpp \
    main.cpp

FORMS   += modbus_test_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/QHexEdit_QT5/QHexEdit_QT5.pri)

include (../lib/MODBUS_server/MODBUS_server.pri)
include (../lib/MODBUS_client/MODBUS_client.pri)

include ($$LIB_PATH2/widgets/hexview16/hexview16.pri)
include ($$LIB_PATH2/widgets/hexspinbox/hexspinbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
