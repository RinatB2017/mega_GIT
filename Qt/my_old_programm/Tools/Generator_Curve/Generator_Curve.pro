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

QT      += multimedia

DEFINES += SAVE_WIDGETS_SLIDER
DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    generator_curve_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    generator_curve_mainbox.cpp \
    main.cpp

FORMS   += generator_curve_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

LIB_PATH  = "../../../lib"
LIB_PATH2 = "../../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

include ($$LIB_PATH2/generator_curve/generator_curve.pri)

include (src/widgets/widgets.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
