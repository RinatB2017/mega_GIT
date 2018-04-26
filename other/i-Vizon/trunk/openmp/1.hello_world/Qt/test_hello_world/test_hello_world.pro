TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -fopenmp
SOURCES += \
    ../../hello_world_openmp.cpp

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

QMAKE_LFLAGS +=  -fopenmp

