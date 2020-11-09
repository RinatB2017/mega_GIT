#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_OpenCV

FOLDER  = tests

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/test_OpenCV_mainbox \
    $$PWD/src/test_OpenCV_mainbox/ui
INCLUDEPATH = $$DEPENDPATH

DEFINES += NO_STYLETOOLBAR
DEFINES += NO_TRAYICON
DEFINES += SHOW_SIZE

#DEFINES += NO_TOOLBAR_BUTTON_EXIT
#DEFINES += NO_TOOLBAR_BUTTON_FONT
#DEFINES += NO_TOOLBAR_BUTTON_LANG
#DEFINES += NO_TOOLBAR_BUTTON_STYLE
#DEFINES += NO_TOOLBAR_BUTTON_ABOUT
#DEFINES += NO_TOOLBAR_BUTTON_HELP
#DEFINES += NO_TOOLBAR_SEPARATORS

DEFINES += PROGRAMM_IN_UTF8

HEADERS += \
    test_OpenCV_mainbox.hpp \
    defines.hpp \
    version.hpp

SOURCES += \
    test_OpenCV_mainbox.cpp \
    main.cpp

FORMS   += test_OpenCV_mainbox.ui

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

unix {
    PKGCONFIG   += opencv4
    CONFIG      += link_pkgconfig
}

win32 {
    INCLUDEPATH += C:/opencv/build/include/
    DEPENDPATH  += C:/opencv/build/include/

    OPENCV_VER = 430
    LIBS    += -L$$PWD/lib
    LIBS    += -lopencv_core$${OPENCV_VER}
    LIBS    += -lopencv_highgui$${OPENCV_VER}
    LIBS    += -lopencv_imgcodecs$${OPENCV_VER}
    LIBS    += -lopencv_imgproc$${OPENCV_VER}
    LIBS    += -lopencv_features2d$${OPENCV_VER}
    LIBS    += -lopencv_calib3d$${OPENCV_VER}
    LIBS    += -lopencv_objdetect$${OPENCV_VER}
}

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
