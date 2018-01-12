#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Test_RGB_Matrix

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/diod \
    $$PWD/src/display \
    $$PWD/src/palette \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

#DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

#DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8

DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

DEFINES += MODBUS

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp \
    palette.hpp \
    display.hpp \
    font-5x7.hpp \
    diod.hpp

SOURCES += \
    mainbox.cpp \
    palette.cpp \
    display.cpp \
    diod.cpp \
    main.cpp \

FORMS += mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}


LIB_PATH = "../../../Qt/lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/crc/crc.pri)

include ($$LIB_PATH/serial5/serial5.pri)
include ($$LIB_PATH/time/time.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
