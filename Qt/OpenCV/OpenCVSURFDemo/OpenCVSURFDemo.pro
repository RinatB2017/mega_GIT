#-------------------------------------------------
#
# Project created by QtCreator 2017-04-15T09:13:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenCVSURFDemo
TEMPLATE = app

SOURCES += main.cpp

win32 {
    INCLUDEPATH += C:/OpenCV/include/
    LIBS += -LC:/OpenCV/x86/mingw/bin/
    OPENCV_VER = 320
}

linux-g++ {
    INCLUDEPATH += $$(HOME)/OpenCV/include/
    LIBS += -L$$(HOME)/OpenCV/lib/
}

LIBS += -lopencv_core$${OPENCV_VER} \
        -lopencv_imgproc$${OPENCV_VER} \
        -lopencv_imgcodecs$${OPENCV_VER} \
        -lopencv_highgui$${OPENCV_VER} \
        -lopencv_flann$${OPENCV_VER} \
        -lopencv_calib3d$${OPENCV_VER} \
        -lopencv_features2d$${OPENCV_VER} \
        -lopencv_xfeatures2d$${OPENCV_VER}
