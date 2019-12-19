#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = MCP4725

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/mcp4725_mainbox \
    $$PWD/src/mcp4725_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    mcp4725_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    mcp4725_mainbox.cpp \
    main.cpp

FORMS   += mcp4725_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH = "../../lib"
LIB_PATH2 = "../../lib2"
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate_win7/serialbox5_fix_baudrate_win7.pri)
include ($$LIB_PATH2/qwt/qwt.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
