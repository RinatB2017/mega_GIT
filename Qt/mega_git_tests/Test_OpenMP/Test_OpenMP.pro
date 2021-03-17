
#CONFIG      += console

TEMPLATE    = app
TARGET      = Test_OpenMP

FOLDER  = tests

include (conf/conf.pri)

DEPENDPATH += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT  += widgets

SOURCES += main.cpp

QMAKE_LFLAGS    += -fopenmp
QMAKE_LIBS      += -lgomp -lpthread
QMAKE_CXXFLAGS  += -fopenmp

VPATH = $$INCLUDEPATH
