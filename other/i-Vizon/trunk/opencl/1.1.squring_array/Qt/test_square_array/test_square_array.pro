TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

iVIZON_TRUNK = /opt/i-vizon/trunk
include(/opt/i-vizon/trunk/opencl/wrapper/clWrapper.pri)
include(/opt/i-vizon/trunk/opencv/wrapper/cvWrapper.pri)



linuxPC {
    copydata.commands = $(COPY_DIR) ../../square_array.cl $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

SOURCES += ../../square_array.cpp
OTHER_FILES += ../../square_array.cl
LIBS += -lrt
