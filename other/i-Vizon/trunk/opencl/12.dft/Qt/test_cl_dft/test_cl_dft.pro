TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(/opt/i-vizon/trunk/opencl/wrapper/clWrapper.pri)
include(/opt/i-vizon/trunk/opencv/wrapper/cvWrapper.pri)

linuxPC {
    copydata.commands = $(COPY_DIR) ../../dft.cl $$DATA/shiva.jpg $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

SOURCES += ../../dft.cpp

OTHER_FILES += ../../dft.cl

