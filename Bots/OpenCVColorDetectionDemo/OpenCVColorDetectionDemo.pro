#-------------------------------------------------
#
# Project created by QtCreator 2017-05-08T09:50:30
#
#-------------------------------------------------

PROGRAMM_PATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenCVColorDetectionDemo
TEMPLATE = app

SOURCES += \
    main.cpp\
    mainwidget.cpp

HEADERS  += mainwidget.h

FORMS    += mainwidget.ui

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
        -lopencv_highgui$${OPENCV_VER}
        
VPATH = $$INCLUDEPATH
