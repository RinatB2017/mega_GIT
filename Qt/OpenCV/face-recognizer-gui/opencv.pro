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
    $$PWD/src \
    $$PWD/gui \
    /usr/include/opencv \
    ./resources

LIBS += -L/lib
LIBS += -I/usr/include/opencv
LIBS += -lopencv_core
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_imgproc
LIBS += -lopencv_objdetect
LIBS += -lopencv_video
LIBS += -lopencv_videoio
LIBS += -lopencv_videostab

#LIBS += -lopencv_cudaimgproc
#LIBS += -lopencv_cudaobjdetect

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    camera.cpp \
    displaywidget.cpp \
    faceDetector.cpp \
    imageviewer.cpp

HEADERS += \
    mainwindow.h \
    camera.h \
    displaywidget.h \
    faceDetector.h \
    imageviewer.h

VPATH = $$INCLUDEPATH
