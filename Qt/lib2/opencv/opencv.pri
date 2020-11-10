#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

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
