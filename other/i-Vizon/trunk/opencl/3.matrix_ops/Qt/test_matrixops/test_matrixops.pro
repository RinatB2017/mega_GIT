TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../../../wrapper/ivizonQt.pri)

imx6qSabreAuto {
    target.files = test_matrixops
    target.path = /home/root/CL
    INSTALLS += target

    copydata.commands = $(COPY_DIR) ../../matrix_ops.cl $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

linuxPC {

    copydata.commands = $(COPY_DIR) ../../matrix_ops.cl $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

SOURCES += ../../matrixops.cpp.cpp

OTHER_FILES += ../../matrix_ops.cl

