TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../../wrapper/clWrapper.pri)
include(../../../opencv/wrapper/cvWrapper.pri)

imx6qSabreAuto {
    target.files = test_sobel_edge_detection
    target.path = /home/root/CL
    INSTALLS += target

    copydata.commands = $(COPY_DIR) ../../image_convolution.cl ../../shiva.jpg $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

linuxPC {
    copydata.commands = $(COPY_DIR) ../sobel_filter.cl ../shiva.jpg $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

SOURCES += ../main.cpp

OTHER_FILES += ../sobel_filter.cl \
               ../shiva.jpg

