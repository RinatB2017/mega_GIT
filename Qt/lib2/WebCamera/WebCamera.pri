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
    INCLUDEPATH += C:\\Users\\User\\OpenCV\\opencv\\build\\include

    LIBS    += -Lc:\\Users\\User\\OpenCV\\opencv\\build\\lib\\Release
    LIBS    += -lopencv_core341
    LIBS    += -lopencv_highgui341
    LIBS    += -lopencv_imgcodecs341
    LIBS    += -lopencv_imgproc341
    LIBS    += -lopencv_features2d341
    LIBS    += -lopencv_calib3d341

#    LIBS += C:\Users\User\OpenCV\opencv\build\bin\Release\opencv_core341.dll
#    LIBS += C:\Users\User\OpenCV\opencv\build\bin\Release\opencv_highgui341.dll
#    LIBS += C:\Users\User\OpenCV\opencv\build\bin\Release\opencv_imgcodecs341.dll
#    LIBS += C:\Users\User\OpenCV\opencv\build\bin\Release\opencv_imgproc341.dll
#    LIBS += C:\Users\User\OpenCV\opencv\build\bin\Release\opencv_features2d341.dll
#    LIBS += C:\Users\User\OpenCV\opencv\build\bin\Release\opencv_calib3d341.dll
}
