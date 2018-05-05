TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../../../wrapper/clWrapper.pri)
include(../../../../opencv/wrapper/cvWrapper.pri)

linuxPC {
    copydata.commands = $(COPY_DIR) \
                        ../../add_image.cl \
                        $$DATA/nature.jpeg \
                        $$DATA/shiva2.jpg \
                        $$DATA/input.bmp $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}


SOURCES += test_image_add_main.cpp

OTHER_FILES += \
    ../../add_image.cl
