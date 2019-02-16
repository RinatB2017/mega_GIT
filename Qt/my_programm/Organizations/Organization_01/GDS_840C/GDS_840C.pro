#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = GDS_840C

FOLDER  = organizations/Organization_01

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/gds_840c_mainbox \
    $$PWD/src/gds_840c_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT      += serialport

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

DEFINES += SAVE_WIDGETS_PROPERTIES
DEFINES += SAVE_WIDGETS_SPINBOX

#DEFINES += FAKE

HEADERS += \
    defines.hpp \
    version.hpp \
    gds_840c_mainbox.hpp

SOURCES += \
    gds_840c_mainbox.cpp \
    main.cpp

FORMS   += gds_840c_mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "../../../../lib"
LIB_PATH2 = "../../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH2/csvreader/csvreader.pri)
include ($$LIB_PATH2/qwt/qwt.pri)

include (../lib/measuring/oscilloscope_GDS_840C/oscilloscope_GDS_840C.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
