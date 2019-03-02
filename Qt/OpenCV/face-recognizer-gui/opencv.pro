#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T18:15:50
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = opencv
TEMPLATE = app

DESTDIR = $$PWD
INCLUDEPATH += \
    /usr/include/opencv \
    ./resources

LIBS += -L/lib
LIBS += -I/usr/include/opencv
LIBS += -lopencv_core
#LIBS += -lopencv_cudaimgproc
#LIBS += -lopencv_cudaobjdetect
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_imgproc
LIBS += -lopencv_objdetect
LIBS += -lopencv_video
LIBS += -lopencv_videoio
LIBS += -lopencv_videostab

SOURCES += \
    main.cpp \
    gui/mainwindow.cpp \
    camera.cpp \
    gui/displaywidget.cpp \
    faceDetector.cpp \
    gui/imageviewer.cpp

HEADERS += \
    gui/mainwindow.h \
    camera.h \
    gui/displaywidget.h \
    faceDetector.h \
    gui/imageviewer.h
