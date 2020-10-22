#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_SerialTerm

FOLDER  = tests

DEPENDPATH  += \
    /usr/include/qtermwidget5 \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += RS232_SEND
DEFINES += NO_LOG

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

LIBS    += -lqtermwidget5

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
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate/serialbox5_fix_baudrate.pri)

include (src/test_serialterm_mainbox/test_serialterm_mainbox.pri)
include (src/serialterm/serialterm.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
