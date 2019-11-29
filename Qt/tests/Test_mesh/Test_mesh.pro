#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_mesh

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/mesh_mainbox \
    $$PWD/src/mesh_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
#DEFINES += SHOW_SIZE
#DEFINES += NO_LOG

#DEFINES += SAVE_INI

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    mesh_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    mesh_mainbox.cpp \
    main.cpp

FORMS   += mesh_mainbox.ui

OTHER_FILES += arduino/Mesh_unit/Mesh_unit.ino
OTHER_FILES += arduino/basic/basic.ino

win32 {
    RC_ICONS += ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

#----------------------------------------------
LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate/serialbox5_fix_baudrate.pri)

include (src/mesh_control/mesh_control.pri)
include (src/mesh_led/mesh_led.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
