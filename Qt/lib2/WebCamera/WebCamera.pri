#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui \
    $$PWD/xml
INCLUDEPATH = $$DEPENDPATH

QT  += opengl

HEADERS += webcamera.hpp
SOURCES += webcamera.cpp
FORMS   += webcamera.ui

HEADERS += cqtopencvviewergl.h
SOURCES += cqtopencvviewergl.cpp

unix {
    PKGCONFIG   += opencv
    CONFIG      += link_pkgconfig
}

win32 {
    INCLUDEPATH += C:\OpenCV\build\include
    DEPENDPATH  += C:\OpenCV\build\include

    LIBS += C:\opencv-build\bin\libopencv_core320.dll
    LIBS += C:\opencv-build\bin\libopencv_highgui320.dll
    LIBS += C:\opencv-build\bin\libopencv_imgcodecs320.dll
    LIBS += C:\opencv-build\bin\libopencv_imgproc320.dll
    LIBS += C:\opencv-build\bin\libopencv_features2d320.dll
    LIBS += C:\opencv-build\bin\libopencv_calib3d320.dll
}
