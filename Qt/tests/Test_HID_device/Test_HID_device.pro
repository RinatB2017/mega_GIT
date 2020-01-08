#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_HID_device

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += SAVE_WIDGETS_SPINBOX
DEFINES += HEX16

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt
#----------------------------------------------
LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/hexspinbox/hexspinbox.pri)
#include ($$LIB_PATH2/QHexEdit_QT5/QHexEdit_QT5.pri)
include (src/HID_device/HID_device.pri)
include (src/AD9106_box/AD9106_box.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------
VPATH = $$INCLUDEPATH
