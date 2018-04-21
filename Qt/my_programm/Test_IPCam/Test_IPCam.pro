#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE    = app
TARGET      = Test_IPCam

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_ip_cam_mainbox \
    $$PWD/src/test_ip_cam_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

QMAKE_CXXFLAGS += -fno-show-column

QT  += network
QT  += multimediawidgets

#DEFINES += RS232_FIXED_SIZE
#DEFINES += RS232_SEND
#DEFINES += RS232_LOG

DEFINES += NO_STYLETOOLBAR
DEFINES += PROGRAMM_IN_UTF8
DEFINES += NO_TRAYICON
#DEFINES += NO_LOG

DEFINES += SAVE_WIDGETS_SPINBOX
DEFINES += SAVE_WIDGETS_SLIDER

HEADERS += \
    defines.hpp \
    version.hpp \
    test_ip_cam_mainbox.hpp

SOURCES += \
    test_ip_cam_mainbox.cpp \
    main.cpp

FORMS   += test_ip_cam_mainbox.ui

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

win32 {
    RC_FILE = ico/myapp.rc
}

LIB_PATH = "../../lib"
LIB_PATH2 = "../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

lessThan(QT_MAJOR_VERSION, 5) {
    error (Only Qt5)
}

VPATH = $$INCLUDEPATH
