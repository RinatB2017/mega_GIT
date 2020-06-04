#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = FindWindow

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/findwindow_mainbox \
    $$PWD/src/findwindow_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += USE_TOPLEVELWIDGETS
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    findwindow_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    findwindow_mainbox.cpp \
    main.cpp

FORMS   += findwindow_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico

    LIBS += -lgdi32
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH    = "$$PWD/../../lib"
LIB_PATH2   = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
