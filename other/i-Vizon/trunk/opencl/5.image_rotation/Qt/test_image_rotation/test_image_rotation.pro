TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#Add CONFIG+=linuxPC or CONFIG+=imx6qSabreAuto in qmake inovocation.
#In qtcreator in left plane goto Projects -> under Build Steps add
#   CONFIG+=linuxPC in Additional Arguments section

imx6qSabreAuto {
    target.files = test_image_rotation #Project Name
    target.path = /home/root/CL
    INSTALLS += target

    otherfiles.files = image_rotation.cl
    otherfiles.path = /home/root/CL
    POST_TARGETDEPS += copyfiles
    INSTALLS += otherfiles


    LIBS += -lOpenCL -lGAL

    LIBS += -lopencv_calib3d \
            -lopencv_contrib \
            -lopencv_core \
            -lopencv_features2d \
            -lopencv_flann \
            -lopencv_highgui\
            -lopencv_imgproc\
            -lopencv_legacy \
            -lopencv_ml\
            -lopencv_nonfree \
            -lopencv_objdetect \
            -lopencv_photo \
            -lopencv_stitching \
            -lopencv_video \
            -lopencv_videostab \

    copydata.commands = $(COPY_DIR) ../../image_rotation.cl ../../shiva.jpg $$OUT_PWD
    first.depends = $(first) copydata
    first.path = /home/root/CL
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

linuxPC {
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    DEFINES += PLATFORM_LINUX EGL_LINUX BOOST_MT
    LIBS += -L/usr/local/lib/
    LIBS += -lGLESv2 -lEGL -lm -lX11 -lboost_system -lboost_thread -lOpenCL

LIBS += -lopencv_calib3d \
        -lopencv_contrib \
        -lopencv_core \
        -lopencv_features2d \
        -lopencv_flann \
        -lopencv_highgui\
        -lopencv_imgproc\
        -lopencv_legacy \
        -lopencv_ml\
        -lopencv_nonfree \
        -lopencv_objdetect \
        -lopencv_photo \
        -lopencv_stitching \
        -lopencv_video \
        -lopencv_videostab \

    INCLUDEPATH += /usr/local/cuda-5.5/include/ #For CL Header on Nvidia Machine

    copydata.commands = $(COPY_DIR) ../../image_rotation.cl ../../shiva.jpg $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}
QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles

########################################################################
#                   i-Vizon Template Section                           #
########################################################################

IVIZON_TRUNK    = /opt/i-vizon/trunk
UTILS           = $$IVIZON_TRUNK/utils
COMMON          = $$IVIZON_TRUNK/common
STDCPLUS          = $$IVIZON_TRUNK/c++11

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Common

HEADERS += $$COMMON/inc/common.h \
            ../../../wrapper/inc/buffer.h \
            ../../../wrapper/inc/kernel_launcher.h \
            ../../../wrapper/inc/program.h \
    ../../../wrapper/inc/sampler.h

SOURCES += $$COMMON/src/common.cpp \
    ../../../wrapper/src/sampler.cpp

INCLUDEPATH += $$COMMON/inc/

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>OpenCL

OPENCL          = $$IVIZON_TRUNK/opencl
OPENCL_WRAPPER  = $$IVIZON_TRUNK/opencl/wrapper

SOURCES += $$OPENCL_WRAPPER/src/cl_wrapper.cpp \
            $$OPENCL_WRAPPER/src/buffer.cpp \
            $$OPENCL_WRAPPER/src/kernel_launcher.cpp \
            $$OPENCL_WRAPPER/src/program.cpp \
            $$OPENCL_WRAPPER/src/image2D.cpp

HEADERS += $$OPENCL_WRAPPER/inc/cl_wrapper.hpp \
            $$OPENCL_WRAPPER/inc/buffer.h\
            $$OPENCL_WRAPPER/inc/kernel_launcher.h \
            $$OPENCL_WRAPPER/inc/program.h \
            $$OPENCL_WRAPPER/inc/image2D.h


INCLUDEPATH += $$OPENCL_WRAPPER/inc/

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>c++11

SOURCES += $$STDCPLUS/file_operation/file_operation.cpp
HEADERS += $$STDCPLUS/file_operation/file_operation.h
INCLUDEPATH += $$STDCPLUS/file_operation/

########################################################################
#                          Project Section                             #
########################################################################

SOURCES += ../../test_image_rotation.cpp

OTHER_FILES += ../../image_rotation.cl \
               ../../shiva.jpg

