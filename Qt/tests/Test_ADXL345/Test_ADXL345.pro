#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_ADXL345

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_ADXL345_mainbox \
    $$PWD/src/test_ADXL345_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += NO_RESIZE

#DEFINES += NO_TOOLBAR
#DEFINES += NO_MENU
#DEFINES += NO_LOG

#DEFINES += LOGO_GL

DEFINES += PROGRAMM_IN_UTF8

#DEFINES += NO_GRAPHER

HEADERS += \
    test_ADXL345_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_ADXL345_mainbox.cpp \
    main.cpp

FORMS   += test_ADXL345_mainbox.ui

OTHER_FILES += \
    doc/notebook.txt \
    arduino/ADXL345_simple/ADXL345_simple.ino

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/temperature.ico
}

RESOURCES += \
    ico/icons.qrc

#----------------------------------------------
LIB_PATH  = "../../../Qt/lib"
LIB_PATH2 = "../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH2/serial5/serial5.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
