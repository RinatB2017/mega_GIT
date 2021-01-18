#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = PTZ_OpenCV

DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    defines.hpp \
    version.hpp

SOURCES += main.cpp

OTHER_FILES += doc/notebook.txt

RESOURCES   += \
    icons/icons.qrc \
    ../common/xml/xml.qrc

win32 {
    RC_ICONS = icons/opencv.ico
}

include ($$PWD/../../../Qt/ext_paths.pri)

include (src/config.pri)
include ($$LIB_PATH/meta/mainwindow.pri)

include ($$LIB_PATH2/icons/arrows.pri)
include ($$LIB_PATH2/widgets/ipv4/ipv4.pri)
include ($$LIB_PATH2/widgets/simple_ptz_widget/simple_ptz_widget.pri)
include ($$LIB_PATH2/widgets/show_hsv_color/show_hsv_color.pri)
include ($$LIB_PATH2/widgets/color_widget/color_widget.pri)
include ($$LIB_PATH2/opencv/opencv.pri)

include (src/ptz_opencv_mainbox/ptz_opencv_mainbox.pri)
include (../common/opencv_widget/opencv_widget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
