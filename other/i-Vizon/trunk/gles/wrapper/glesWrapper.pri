####################################
# GLES Template Section            #
####################################

IVIZON_TRUNK = /opt/i-vizon/trunk
GLES	     = $$IVIZON_TRUNK/gles
GLES_WRAPPER = $$GLES/wrapper/
UTILS 	     = $$IVIZON_TRUNK/utils
COMMON       = $$IVIZON_TRUNK/common
DEFINES     += IVIZON_DEBUG

linuxPC {
    DEFINES += EGL_LINUX BOOST_MT
    LIBS += -L/usr/local/lib/
    LIBS += -lGLESv2 -lEGL -lm -lX11 -lboost_system -lboost_thread

    INCLUDEPATH += /usr/local/include
}

SOURCES += $$GLES_WRAPPER/src/iv_gles_wrapper.cpp \
           $$GLES_WRAPPER/src/iv_egl_linux_api.cpp \
           $$GLES_WRAPPER/src/iv_gles_api.cpp \
           $$UTILS/src/MatrixOperations.cpp \
           $$UTILS/src/LinuxTimer.cpp


HEADERS += $$GLES_WRAPPER/inc/iv_gles_wrapper.h \
           $$COMMON/inc/iv_common.h \
           $$UTILS/inc/MatrixOperations.h \
           $$UTILS/inc/LinuxTimer.h


INCLUDEPATH += $$GLES_WRAPPER/inc/ \
               $$UTILS/inc \
               $$COMMON/inc

