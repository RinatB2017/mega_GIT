#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_NeuronNet

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/test_NeuronNet_mainbox \
    $$PWD/src/test_NeuronNet_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_NeuronNet_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_NeuronNet_mainbox.cpp \
    main.cpp

FORMS   += test_NeuronNet_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += \
    images/images.qrc \
    data_NMIST/data_nmist.qrc

OTHER_FILES += doc/notebook.txt

LIB_PATH = "../../../Qt/lib"
include ($$LIB_PATH/meta/mainwindow.pri)
include (src/neuro/neuro.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

CONFIG(debug, debug|release) {
    message(debug mode)
}
else {
    message(release mode)
}

VPATH = $$INCLUDEPATH
