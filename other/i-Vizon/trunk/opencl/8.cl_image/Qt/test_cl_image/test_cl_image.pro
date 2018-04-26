TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
PKGCONFIG += opencv

LIBS += -L/usr/local/lib/\
        -lOpenCL

SOURCES += \
    ../../cl_image_rw_main.cpp

OTHER_FILES += \ 
    ../../imgRW.cl

INCLUDEPATH += /usr/local/cuda-5.5/include/

copydata.commands = $(COPY_DIR) ../../imgRW.cl \
                    ../../../../data/shiva1.jpg \
                    ../../../../data/shiva.jpg $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
