#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Copyrighter

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/copyrighter_mainbox \
    $$PWD/src/copyrighter_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += USE_TOPLEVELWIDGETS
#DEFINES += FIXED_SIZE

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    copyrighter_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    copyrighter_mainbox.cpp \
    main.cpp

FORMS += copyrighter_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

unix {
    LIBS += -lhunspell
}

win32 {
    INCLUDEPATH += C:/path/to/hunspell/include
    LIBS += C:/path/to/hunspell/Release/hunspell.lib
}

LIB_PATH  = "$$PWD/../../../lib"
LIB_PATH2 = "$$PWD/../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/smtp/smtp.pri)

include (src/mytextedit/mytextedit.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
