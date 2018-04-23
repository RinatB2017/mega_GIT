TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
PKGCONFIG += opencv

LIBS += -L/usr/local/lib/\
        -lOpenCL

SOURCES += main.cpp

HEADERS +=  main.h \
            rotation.cl

INCLUDEPATH += /usr/include/
