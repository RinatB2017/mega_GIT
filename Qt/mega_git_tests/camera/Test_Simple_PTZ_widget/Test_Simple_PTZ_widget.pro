#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_Simple_PTZ_widget

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += main.cpp

OTHER_FILES += doc/notebook.txt

RESOURCES   += ico/icons.qrc

win32 {
    RC_ICONS = ico/camera.ico
}

LIB_PATH  = "$$PWD/../../../../Qt/lib"
LIB_PATH2 = "$$PWD/../../../../Qt/lib2"

include (src/config.pri)
include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/icons/arrows.pri)
include ($$LIB_PATH2/widgets/ipv4/ipv4.pri)
include ($$LIB_PATH2/widgets/simple_ptz_widget/simple_ptz_widget.pri)
include ($$LIB_PATH2/opencv/opencv.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
