#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_CRC

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += SHOW_SIZE

#DEFINES += NO_TOOLBAR_BUTTON_EXIT
#DEFINES += NO_TOOLBAR_BUTTON_FONT
#DEFINES += NO_TOOLBAR_BUTTON_LANG
#DEFINES += NO_TOOLBAR_BUTTON_STYLE
#DEFINES += NO_TOOLBAR_BUTTON_ABOUT
#DEFINES += NO_TOOLBAR_BUTTON_HELP
#DEFINES += NO_TOOLBAR_SEPARATORS

DEFINES += PROGRAMM_IN_UTF8

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
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/crc/crc.pri)

include (src/test_CRC_mainbox/test_CRC_mainbox.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
