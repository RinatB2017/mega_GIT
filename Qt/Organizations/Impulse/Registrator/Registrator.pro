#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Registrator

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/mainbox \
    $$PWD/src/mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

QT      += serialport

DEFINES += USE_SCALE_POINT_DATETIME
DEFINES += NO_TESTBAR_GRAPHER
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += CONVERT_DATA_TO_ASCII
DEFINES += PROGRAMM_IN_UTF8

DEFINES	+= FAKE

HEADERS += \
    defines.hpp \
    version.hpp \
    mainbox.hpp

SOURCES += \
    mainbox.cpp \
    main.cpp

FORMS   += mainbox.ui

OTHER_FILES += doc/notebook.txt

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../lib"

include (src/multimeter_frame/multimeter_frame.pri)

include ($$LIB_PATH/locale.pri)
include ($$LIB_PATH/qtsingleapplication/qtsingleapplication.pri)
include ($$LIB_PATH/mywaitsplashscreen/mywaitsplashscreen.pri)
include ($$LIB_PATH/mysplashscreen/mysplashscreen.pri)
include ($$LIB_PATH/power_knock/power_knock.pri)
include ($$LIB_PATH/mainwindow/mainwindow.pri)
include ($$LIB_PATH/GrapherBox/GrapherBox.pri)
include ($$LIB_PATH/csvreader/csvreader.pri)
include ($$LIB_PATH/icons/digits.pri)
include ($$LIB_PATH/qwt/qwt.pri)
include ($$LIB_PATH/crc/crc.pri)
include ($$LIB_PATH/turbo.pri)

include ($$LIB_PATH/measuring/multimeter_v786_2/multimeter_v786_2.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
