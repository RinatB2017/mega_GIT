#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Check_Network
CONFIG      += c++11

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/check_network_mainbox \
    $$PWD/src/check_network_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QT  += network

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += PROGRAMM_IN_UTF8
#DEFINES += NO_LOG
#DEFINES += NO_RESIZE

#DEFINES += USE_RTSP

DEFINES += SAVE_WIDGETS_SPINBOX

HEADERS += \
    check_network_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    check_network_mainbox.cpp \
    main.cpp

FORMS   += check_network_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

RESOURCES += ico/icons.qrc

RESOURCES += \
    images/images.qrc \
    doc/doc.qrc

OTHER_FILES += doc/notebook.txt

LIB_PATH  = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/widgets/ipctrl4/ipctrl4.pri)
include ($$LIB_PATH2/widgets/rtsp_widget/rtsp_widget.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
