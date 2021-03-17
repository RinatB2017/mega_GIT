#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_ADC

FOLDER  = tests

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
LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/meta/grapherbox.pri)
include ($$LIB_PATH2/DataGrapherBox/DataGrapherBox.pri)

#----------------------------------------------
CONFIG  += use_worker
use_worker {
    include ($$LIB_PATH2/serial5/fake_serialwidget/serialwidget/serialwidget.pri)
    include (src/worker_fake/worker_fake.pri)
} else {
    include ($$LIB_PATH2/serial5/serialwidget/serialwidget.pri)
}
include ($$LIB_PATH2/serial5/serial5.pri)
#----------------------------------------------

include (src/test_ADC_mainbox/test_ADC_mainbox.pri)

#!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    message($$VERSION_HEADER)
#    include ($$LIB_PATH/auto_inc_version.pri)
#}
#----------------------------------------------

VPATH = $$INCLUDEPATH
