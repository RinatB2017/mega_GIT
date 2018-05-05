TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../../../wrapper/clWrapper.pri)
include(../../../../opencv/wrapper/cvWrapper.pri)

imx6qSabreAuto {
    target.files = test_c_code
    target.path = /home/root/CL
    INSTALLS += target

    #copydata.commands = $(COPY_DIR) ../../rotation.cl ../../shiva.jpg $$OUT_PWD
    copydata.commands = $(COPY_DIR) ../../intrep.cl ../../input.png $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata

    LIBS += -lpng
}

linuxPC {
 #   copydata.commands = $(COPY_DIR) ../../rotation.cl ../../shiva.jpg $$OUT_PWD
     copydata.commands = $(COPY_DIR) ../../intrep.cl ../../input1.png ../../input.png $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
    LIBS += -lpng
}

SOURCES += \
    ../../test_c_code.cpp
OTHER_FILES += ../../intrep.cl

