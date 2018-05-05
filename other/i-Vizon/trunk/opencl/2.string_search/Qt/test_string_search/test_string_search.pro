TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../../../wrapper/ivizonQt.pri)

#Add CONFIG+=linuxPC or CONFIG+=imx6qSabreAuto in qmake inovocation.
#In qtcreator in left plane goto Projects -> under Build Steps add
#   CONFIG+=linuxPC in Additional Arguments section

imx6qSabreAuto {
    target.files = test_string_search
    target.path = /home/root/CL
    INSTALLS += target
    LIBS += -lOpenCL -lGAL

    copydata.commands = $(COPY_DIR) ../../string_search.cl ../../string_search.txt $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

linuxPC {
    QT_CONFIG -= no-pkg-config
    CONFIG += link_pkgconfig
    #PKGCONFIG += opencv
    DEFINES += PLATFORM_LINUX EGL_LINUX BOOST_MT
    LIBS += -L/usr/local/lib/
    LIBS += -lGLESv2 -lEGL -lm -lX11 -lboost_system -lboost_thread -lOpenCL
    INCLUDEPATH += /usr/local/cuda-5.5/include/ #For CL Header on Nvidia Machine

    copydata.commands = $(COPY_DIR) ../../string_search.cl ../../string_search.txt $$OUT_PWD
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}


SOURCES += main.cpp \
            ../../string_search.cpp

OTHER_FILES += ../../string_search.cl \
               ../../string_search.txt


