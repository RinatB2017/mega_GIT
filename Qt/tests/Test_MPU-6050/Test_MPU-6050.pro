#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_MPU-6050

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_MPU-6050_mainbox \
    $$PWD/src/test_MPU-6050_mainbox/ui
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
    test_MPU-6050_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_MPU-6050_mainbox.cpp \
    main.cpp

FORMS   += test_MPU-6050_mainbox.ui

OTHER_FILES += \
    doc/notebook.txt \
    arduino/MPU-6050/MPU-6050.ino \
    example/scepter/scepter.ino \
    example/gy_521_send_serial/gy_521_send_serial.ino \
    example/ShowGY521Data/ShowGY521Data.pde \
    example/43/43.c \
    example/43/bcd.c \
    example/43/bcd.h \
    example/43/compilers_4.h \
    example/43/i2c.h \
    example/43/lcd_lib_2.c \
    example/43/lcd_lib_2.h \
    example/43/port_macros.h

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/temperature.ico
}

include (src/glwidget/glwidget_qt5.pri)

QT *= opengl

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
