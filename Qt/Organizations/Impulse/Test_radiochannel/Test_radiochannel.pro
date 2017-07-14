#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_radiochannel

FOLDER  = organizations/Impulse

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

QT      += serialport

DEFINES += USE_SCALE_POINT_TIME

DEFINES += SAVE_WIDGETS_PROPERTIES
DEFINES += SAVE_WIDGETS_SPINBOX

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS += mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"

include ($$LIB_PATH/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH/csvreader/csvreader.pri)
include ($$LIB_PATH/time/time.pri)
include ($$LIB_PATH/qwt/qwt.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
