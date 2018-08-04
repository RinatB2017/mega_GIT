#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Generator_Curve

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/generator_curve_mainbox \
    $$PWD/src/generator_curve_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

#QMAKE_CXXFLAGS += -fno-show-column

DEFINES += SAVE_WIDGETS_SLIDER
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp \
    generator_curve_mainbox.hpp

SOURCES += \
    generator_curve_mainbox.cpp \
    main.cpp

FORMS   += generator_curve_mainbox.ui

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include ($$LIB_PATH2/generator_curve/generator_curve.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
