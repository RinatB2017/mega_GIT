TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src \
	$$PWD/opencl
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QT_CONFIG -= no-pkg-config

SOURCES += main.cpp

HEADERS +=  main.h \
            rotation.cl

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
    INCLUDEPATH += /usr/include/

    LIBS += -L/usr/local/lib/
    LIBS += -lOpenCL
}

VPATH = $$INCLUDEPATH
