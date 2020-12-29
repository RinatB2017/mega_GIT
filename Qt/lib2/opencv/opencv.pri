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

CONFIG(debug, debug|release) {
    OPENCV_VER = 430d
    message (debug)
} else {
    OPENCV_VER = 430
    message (release)
}
#    LIBS    += -lgdi32

    LIBS    += -L$$PWD/lib

    LIBS    += -lopencv_core$${OPENCV_VER}
    LIBS    += -lopencv_highgui$${OPENCV_VER}
    LIBS    += -lopencv_imgcodecs$${OPENCV_VER}
    LIBS    += -lopencv_imgproc$${OPENCV_VER}
    LIBS    += -lopencv_features2d$${OPENCV_VER}
    LIBS    += -lopencv_calib3d$${OPENCV_VER}
    LIBS    += -lopencv_objdetect$${OPENCV_VER}

    LIBS    += -lopencv_flann$${OPENCV_VER}
    LIBS    += -lopencv_stitching$${OPENCV_VER}
    LIBS    += -lopencv_gapi$${OPENCV_VER}
    LIBS    += -lopencv_ml$${OPENCV_VER}
    LIBS    += -lopencv_ts$${OPENCV_VER}
    LIBS    += -lopencv_dnn$${OPENCV_VER}
    LIBS    += -lopencv_video$${OPENCV_VER}
    LIBS    += -lopencv_photo$${OPENCV_VER}
    LIBS    += -lopencv_videoio$${OPENCV_VER}

    LIBS    += -lopencv_aruco$${OPENCV_VER}
}
