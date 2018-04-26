TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../../../wrapper/clWrapper.pri)
include(../../../../opencv/wrapper/cvWrapper.pri)

imx6qSabreAuto {
    target.files = test_image_convolution
    target.path = /home/root/CL
    INSTALLS += target

    copydata.commands = $(COPY_DIR) ../../image_convolution.cl ../../shiva.jpg $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

linuxPC {
    copydata.commands = $(COPY_DIR) \
                        ../../image_convolution.cl \
                        $$DATA/shiva1.jpg \
                        $$DATA/shiva.jpg \
                        $$DATA/input.bmp $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

SOURCES += \
    ../../image_Convolution.cpp

OTHER_FILES += ../../shiva.jpg \
    ../../image_convolution.cl
