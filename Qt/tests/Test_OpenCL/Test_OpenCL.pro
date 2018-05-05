TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#    /usr/lib/OpenCL/vendors/mesa/include \
#    /usr/lib/gcc/x86_64-pc-linux-gnu/7.3.0/include/g++-v7 \
#CONFIG      += link_pkgconfig
#INCLUDEPATH += /usr/include/

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/opencl
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QT_CONFIG -= no-pkg-config

#HEADERS     += hello.cl
#SOURCES     += main.cpp

HEADERS     += HelloWorld.cl
SOURCES     += HelloWorld.cpp

unix {
    LIBS += -L/usr/local/lib/
    LIBS += -lOpenCL
}

VPATH = $$INCLUDEPATH
