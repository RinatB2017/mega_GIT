####################################
# Template Section                 #
####################################

IVIZON_TRUNK = /opt/i-vizon/trunk
GLES = $$IVIZON_TRUNK/gles
GLES_WRAPPER = $$GLES/Wrapper/
UTILS = $$IVIZON_TRUNK/utils
COMMON = $$IVIZON_TRUNK/common
unix {
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    DEFINES += IVIZON_DEBUG EGL_LINUX BOOST_MT
    LIBS += -L/usr/local/lib/
    LIBS += -lGLESv2 -lEGL -lm -lX11 -lboost_system -lboost_thread
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

CUBETEXTURE = $$GLES/2.CubeTexture/


HEADERS += $$CUBETEXTURE/cube_texture.h

SOURCES += main.cpp \
            $$CUBETEXTURE/cube_texture.cpp

OTHER_FILES += \
    ../../cubeFragment.fsh \
    ../../cubeVertex.vsh

