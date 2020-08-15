#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_QGraphicsView

FOLDER  = tests

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/test_QGraphicsView_mainbox \
    $$PWD/src/test_QGraphicsView_mainbox/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

#QT  += serialbus

#DEFINES += LOGO_GL
#DEFINES += FIXED_SIZE

#
DEFINES += NO_STYLETOOLBAR
#DEFINES += NO_STATUSBAR
DEFINES += NO_TRAYICON
#DEFINES += NO_TOOLBAR
#DEFINES += NO_MENU
#DEFINES += NO_LOG

#DEFINES += ONLY_ENGLISH

#DEFINES += LOG_READ_ONLY

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_QGraphicsView_mainbox.hpp \
    defines.hpp \
    version.hpp \

SOURCES += \
    test_QGraphicsView_mainbox.cpp \
    main.cpp

FORMS   += test_QGraphicsView_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
