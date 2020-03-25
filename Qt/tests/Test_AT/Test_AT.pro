#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Test_AT

FOLDER      = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_at_mainbox \
    $$PWD/src/test_at_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

#DEFINES += SAVE_INI

HEADERS += \
    test_at_mainbox.hpp \
    defines.hpp \
    version.hpp \

SOURCES += \
    test_at_mainbox.cpp \
    main.cpp

FORMS   += test_at_mainbox.ui

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/RS232.ico
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serialbox5_lite/serialbox5_lite.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

lessThan(QT_MAJOR_VERSION, 5) {
    error (Only Qt5)
}

RESOURCES += \
    ico/icons.qrc

VPATH = $$INCLUDEPATH

