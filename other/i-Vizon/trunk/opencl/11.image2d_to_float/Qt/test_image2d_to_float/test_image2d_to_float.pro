TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../../../wrapper/clWrapper.pri)
include(../../../../opencv/wrapper/cvWrapper.pri)

linuxPC {
    copydata.commands = $(COPY_DIR) \
                        ../../image2d_to_float.cl \
                        $$DATA/shiva46x46.jpg \
                        $$DATA/nature.jpeg \
                        $$DATA/shiva2.jpg \
                        $$DATA/input.bmp $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

SOURCES += \
    ../../image2d_to_float_main.cpp

OTHER_FILES += ../../image2d_to_float.cl
