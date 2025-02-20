#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Sensors_DS16B20

include (conf/conf.pri)

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp

SOURCES += \
    main.cpp

OTHER_FILES += \
    doc/notebook.txt \
    arduino/arduino.ino

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_ICONS = ico/I2c_logo.ico
}

RESOURCES += \
    ico/icons.qrc

#----------------------------------------------
win32 {
    include (Y:/ext_paths.pri)
} else {
    include ($$(HOME)/Programming/_Github/private/RIP/ext_paths.pri)
}
#----------------------------------------------
include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH2/DataGrapherBox/DataGrapherBox.pri)
#----------------------------------------------
#CONFIG  += use_worker
use_worker {
    include ($$LIB_PATH2/serial5/fake_serialwidget/serialwidget/serialwidget.pri)
    include (src/worker_fake/worker_fake.pri)
} else {
    include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
}
include ($$LIB_PATH2/serial5/serialbox5_fix_baudrate/serialbox5_fix_baudrate.pri)
#----------------------------------------------
include (src/sensors_DS16B20_mainbox/sensors_DS16B20_mainbox.pri)
----------------------------------------------

VPATH = $$INCLUDEPATH
