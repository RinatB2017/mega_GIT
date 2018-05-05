####################################
# GLES Template Section            #
####################################

IVIZON_TRUNK = /opt/i-vizon/trunk
GLES = $$IVIZON_TRUNK/gles
GLES_WRAPPER = $$GLES/wrapper/
UTILS = $$IVIZON_TRUNK/utils
COMMON = $$IVIZON_TRUNK/common

linuxPC {
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    DEFINES += IVIZON_DEBUG EGL_LINUX BOOST_MT
    LIBS += -L/usr/local/lib/
    LIBS += -lGLESv2 -lEGL -lm -lX11 -lboost_system -lboost_thread

    INCLUDEPATH += /usr/local/include/opencv\
                /usr/local/include


    LIBS += -L/usr/local/lib/\
            -lopencv_calib3d\
            -lopencv_contrib\
            -lopencv_core\
            -lopencv_features2d\
            -lopencv_flann\
            -lopencv_highgui\
            -lopencv_imgproc\
            -lopencv_legacy\
            -lopencv_ml\
            -lopencv_nonfree\
            -lopencv_objdetect\
            -lopencv_photo\
            -lopencv_stitching\
            -lopencv_ts\
            -lopencv_video\
            -lopencv_videostab\
}

SOURCES += $$GLES_WRAPPER/src/GLESWrapper.cpp \
           $$GLES_WRAPPER/src/EGLLinuxAPIs.cpp \
           $$GLES_WRAPPER/src/GLESAPIs.cpp \
           $$UTILS/src/MatrixOperations.cpp \
           $$UTILS/src/LinuxTimer.cpp


HEADERS += $$GLES_WRAPPER/inc/GLESWrapper.h \
           $$COMMON/inc/Common.h \
           $$UTILS/inc/MatrixOperations.h \
           $$UTILS/inc/LinuxTimer.h


INCLUDEPATH += $$GLES_WRAPPER/inc/ \
               $$UTILS/inc \
               $$COMMON/inc


####################################
# Project Section                  #
####################################
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += $$GLES/3.Rectangle/main.cpp

OTHER_FILES += \
    ../../fRectangle.fsh \
    ../../vRectangle.vsh

