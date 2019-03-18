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

#    LIBS += C:\Users\User\OpenCV\opencv\build\bin\Release\opencv_core341.dll
#    LIBS += C:\Users\User\OpenCV\opencv\build\bin\Release\opencv_highgui341.dll
#    LIBS += C:\Users\User\OpenCV\opencv\build\bin\Release\opencv_imgcodecs341.dll
#    LIBS += C:\Users\User\OpenCV\opencv\build\bin\Release\opencv_imgproc341.dll
#    LIBS += C:\Users\User\OpenCV\opencv\build\bin\Release\opencv_features2d341.dll
#    LIBS += C:\Users\User\OpenCV\opencv\build\bin\Release\opencv_calib3d341.dll
}
