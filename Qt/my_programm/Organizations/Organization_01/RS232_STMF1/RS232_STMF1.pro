#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET = RS232_STMF1

FOLDER  = organizations/Organization_01

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/rs232_stmf1_mainbox \
    $$PWD/src/rs232_stmf1_mainbox/ui \
    $$PWD/src/controlbox \
    $$PWD/src/controlbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

QT      += serialport

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

#DEFINES += FAKE

HEADERS += \
    defines.hpp \
    version.hpp \
    rs232_stmf1_mainbox.hpp \
    controlbox.hpp \

SOURCES += \
    rs232_stmf1_mainbox.cpp \
    main.cpp \
    controlbox.cpp

FORMS   += \
    rs232_stmf1_mainbox.ui \
    controlbox.ui

OTHER_FILES	+= notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

include (src/test_serialport/testserialport.pri)

LIB_PATH = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH/time/time.pri)
include ($$LIB_PATH2/qwt/qwt.pri)
include ($$LIB_PATH2/crc/crc.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
