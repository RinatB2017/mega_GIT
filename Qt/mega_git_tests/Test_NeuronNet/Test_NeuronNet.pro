#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_NeuronNet

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += \
    main.cpp

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc \
    data_NMIST/data_nmist.qrc

OTHER_FILES += doc/notebook.txt

LIB_PATH = "$$PWD/../../../Qt/lib"

include ($$LIB_PATH/meta/mainwindow.pri)
include (src/test_NeuronNet_mainbox/test_NeuronNet_mainbox.pri)
include (src/neuro/neuro.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

CONFIG(debug, debug|release) {
    message(debug mode)
}
else {
    message(release mode)
}

VPATH = $$INCLUDEPATH
