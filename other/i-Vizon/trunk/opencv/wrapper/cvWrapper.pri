########################################################################
#                   i-Vizon OpenCV Section                             #
########################################################################

linuxPC {
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
}

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>OpenCV

OPENCV          = $$IVIZON_TRUNK/opencv
OPENCV_WRAPPER  = $$OPENCV/wrapper

HEADERS += $$OPENCV_WRAPPER/inc/cv_wrapper.h

SOURCES += $$OPENCV_WRAPPER/src/cv_wrapper.cpp

INCLUDEPATH += $$OPENCV_WRAPPER/inc/

#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<OpenCV


INCLUDEPATH += $$OPENCL_WRAPPER/inc/


LIBS += -lopencv_calib3d \
        -lopencv_contrib \
        -lopencv_core \
        -lopencv_features2d \
        -lopencv_flann \
        -lopencv_highgui\
        -lopencv_imgproc\
        -lopencv_legacy \
        -lopencv_ml\
        -lopencv_nonfree \
        -lopencv_objdetect \
        -lopencv_photo \
        -lopencv_stitching \
        -lopencv_video \
        -lopencv_videostab \

