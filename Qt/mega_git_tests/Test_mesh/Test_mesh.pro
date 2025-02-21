#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_mesh

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

OTHER_FILES += arduino/Mesh_unit/Mesh_unit.ino
OTHER_FILES += arduino/basic/basic.ino

win32 {
    RC_ICONS += ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

#----------------------------------------------
win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate/serialbox5_fix_baudrate.pri)

include (src/mesh_mainbox/mesh_mainbox.pri)
include (src/mesh_control/mesh_control.pri)
include (src/mesh_led/mesh_led.pri)
#----------------------------------------------

VPATH = $$INCLUDEPATH
