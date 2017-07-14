#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET = RS232_STMF0

FOLDER  = organizations/Impulse

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui \
    $$PWD/src/controlbox \
    $$PWD/src/controlbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += FLAG_RESIZE
DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp \
    controlbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp \
    controlbox.cpp

FORMS += \
    mainbox.ui \
    controlbox.ui

OTHER_FILES	+= notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH/csvreader/csvreader.pri)
include ($$LIB_PATH/serial/serial.pri)
include ($$LIB_PATH/time/time.pri)
include ($$LIB_PATH/qwt/qwt.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
#message ($$QT)
