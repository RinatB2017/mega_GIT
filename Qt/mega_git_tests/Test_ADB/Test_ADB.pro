#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE = app
TARGET   = Test_ADB

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
    INCLUDEPATH += C:/OpenCV/include/
    LIBS += -LC:/OpenCV/x86/mingw/bin/
    OPENCV_VER = 410 #320

    LIBS += -lopencv_core$${OPENCV_VER} \
            -lopencv_highgui$${OPENCV_VER} \
            -lopencv_imgproc$${OPENCV_VER} \
            -lopencv_imgcodecs$${OPENCV_VER} \
            -lopencv_features2d$${OPENCV_VER} \
            -lopencv_calib3d$${OPENCV_VER}
}

#linux-g++ {
#    INCLUDEPATH += $$(HOME)/OpenCV/include/
#    LIBS += -L$$(HOME)/OpenCV/lib/
#}

unix {
    PKGCONFIG   += opencv4
    CONFIG      += link_pkgconfig
}

win32 {
    RC_ICONS = ico/computer.ico
}

# не забыть при смене Qt изменить файлы в каталоге win
RESOURCES += \
    ../../lib2/icons/arrows/arrows.qrc \
    images/images.qrc

OTHER_FILES += doc/notebook.txt

CONFIG(debug, debug|release) {
    include (src/test/test.pri)
}

LIB_PATH  = "$$PWD/../../../Qt/lib"

include ($$LIB_PATH/meta/mainwindow.pri)

include (src/test_adb_mainbox/test_adb_mainbox.pri)

include (src/autoclicker/autoclicker.pri)

!exists(OBJECTS_DIR) {
    VERSION_HEADER = $$PWD/src/version.hpp
    include ($$LIB_PATH/auto_inc_version.pri)
}

VPATH = $$INCLUDEPATH
