#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Copyrighter

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/copyrighter_mainbox \
    $$PWD/src/copyrighter_mainbox/ui \
    $$PWD/src/mytextedit
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += USE_TOPLEVELWIDGETS
#DEFINES += FIXED_SIZE

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    copyrighter_mainbox.hpp \
    defines.hpp \
    version.hpp \
    mytextedit.hpp

SOURCES += \
    copyrighter_mainbox.cpp \
    mytextedit.cpp \
    main.cpp

FORMS += copyrighter_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/smtp/smtp.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
