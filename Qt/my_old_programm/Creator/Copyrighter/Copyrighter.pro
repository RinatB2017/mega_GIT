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

#DEFINES += FIXED_SIZE

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    copyrighter_mainbox.hpp \
    mytextedit.hpp

SOURCES += \
    copyrighter_mainbox.cpp \
    main.cpp \
    mytextedit.cpp

FORMS += copyrighter_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH = "$$PWD/../../../lib"

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/mywidgets/mywidget.pri)
include ($$LIB_PATH/turbo.pri)

include ($$LIB_PATH/smtp/smtp.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
