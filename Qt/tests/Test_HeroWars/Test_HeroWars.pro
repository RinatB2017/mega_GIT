#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_HeroWars

FOLDER  = tests

DEPENDPATH  += \
    $$PWD \
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

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    targets/targets.qrc

OTHER_FILES += doc/notebook.txt

unix {
    CONFIG  += c++11

    PKGCONFIG   += opencv
    CONFIG      += link_pkgconfig

    LIBS    += -lX11 -lXtst
    LIBS    += -lOpenCL
}
win32 {
    INCLUDEPATH += C:\\OpenCV\\include

    OPENCV_VER = 410
    LIBS    += -LC:\\OpenCV\\x86\\mingw\\bin
    LIBS    += -lopencv_core$${OPENCV_VER}
    LIBS    += -lopencv_highgui$${OPENCV_VER}
    #LIBS    += -lopencv_imgcodecs$${OPENCV_VER}
    LIBS    += -lopencv_imgproc$${OPENCV_VER}
    LIBS    += -lopencv_features2d$${OPENCV_VER}
    LIBS    += -lopencv_calib3d$${OPENCV_VER}
    LIBS    += -lopencv_video$${OPENCV_VER}
    LIBS    += -lopencv_videoio$${OPENCV_VER}
    LIBS    += -lopencv_objdetect$${OPENCV_VER}
}

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../lib"
LIB_PATH2 = "$$PWD/../../lib2"

include ($$LIB_PATH/meta/mainwindow.pri)
include ($$LIB_PATH2/widgets/showpicture/showpicture.pri)

include (src/myfindform/myfindform.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
