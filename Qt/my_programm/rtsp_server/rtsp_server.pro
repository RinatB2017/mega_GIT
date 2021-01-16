QT -= gui

CONFIG += c++11
CONFIG -= app_bundle

SOURCES += \
        main.cpp

INCLUDEPATH += \
    /usr/include/gstreamer-1.0 \
    /usr/lib/glib-2.0/include \
    /usr/include/glib-2.0 \
#    /usr/include/libxml2

LIBS    += -lgthread-2.0
LIBS    += -lglib-2.0
LIBS    += -lgstrtspserver-1.0
LIBS    += -lpthread
LIBS    += -lgstreamer-1.0
LIBS    += -ldbus-glib-1
LIBS    += -ldbus-1

VPATH = $$INCLUDEPATH
