TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

PROGRAMM_PATH  += \
    /usr/include \
    /usr/lib/OpenCL/global/include/CL-2.1 \
    /usr/lib/gcc/x86_64-pc-linux-gnu/8.1.0/include/g++-v8 \
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
