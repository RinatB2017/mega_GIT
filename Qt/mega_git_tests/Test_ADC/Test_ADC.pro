#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_ADC

FOLDER  = tests

VER_MAJOR = 1
VER_MINOR = 0
VER_PATCH = 0
VER_BUILD = 0

DEFINES += VER_MAJOR=$${VER_MAJOR}
DEFINES += VER_MINOR=$${VER_MINOR}
DEFINES += VER_BUILD=$${VER_BUILD}
DEFINES += VER_PATCH=$${VER_PATCH}

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
    VERSION = $${VER_MAJOR}"."$${VER_MINOR}"."$${VER_PATCH}"."$${VER_BUILD}

    QMAKE_TARGET_COMPANY = Home
    QMAKE_TARGET_PRODUCT = $$TARGET
    QMAKE_TARGET_COPYRIGHT = "Copyright \\251 2020-2025"
    QMAKE_TARGET_DESCRIPTION = "my description"

    RC_ICONS = ico/I2c_logo.ico
}

RESOURCES += \
    ico/icons.qrc

#----------------------------------------------
LIB_PATH  = "$$PWD/../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../Qt/lib2"

include (src/config.pri)    #строка должна быть перед mainwindow
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

!exists(OBJECTS_DIR) {
#    VERSION_HEADER = $$PWD/src/version.hpp
#    message($$VERSION_HEADER)
#    include ($$LIB_PATH/auto_inc_version.pri)
}
#----------------------------------------------

VPATH = $$INCLUDEPATH
