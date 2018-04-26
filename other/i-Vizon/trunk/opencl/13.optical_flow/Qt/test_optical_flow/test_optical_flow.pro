TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(/opt/i-vizon/trunk/opencl/wrapper/clWrapper.pri)
include(/opt/i-vizon/trunk/opencv/wrapper/cvWrapper.pri)

linuxPC {
    copydata.commands = $(COPY_DIR) ../../optical_flow.cl $$DATA/basketball1.png \
        $$DATA/basketball2.png \
        $$DATA/rubberwhale1.png \
        $$DATA/rubberwhale2.png \
        $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}


SOURCES += ../../optical_flow.cpp
OTHER_FILES += ../../optical_flow.cl
