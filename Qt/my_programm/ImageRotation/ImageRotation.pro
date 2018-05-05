TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

PROGRAMM_PATH  += \
#    C:\OpenCV\sources\3rdparty\include\opencl\1.2 \
#    c:\OpenCV\sources\include \
#    c:\OpenCV\sources\modules\core\include \
#    c:\OpenCV\build\include \
    /usr/lib/gcc/x86_64-pc-linux-gnu/7.3.0/include/g++-v7 \
    /usr/lib/OpenCL/vendors/mesa/include \
    $$PWD \
    $$PWD/src \
    $$PWD/opencl
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QT_CONFIG -= no-pkg-config

SOURCES += main.cpp

HEADERS +=  \
    main.h \
    rotation.cl

unix {
    CONFIG      += link_pkgconfig
    PKGCONFIG   += opencv
    INCLUDEPATH += /usr/include/

    LIBS += -L/usr/local/lib/
    LIBS += -lOpenCL
}

VPATH = $$INCLUDEPATH
