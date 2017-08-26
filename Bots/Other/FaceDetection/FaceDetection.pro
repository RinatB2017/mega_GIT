
TARGET      = FaceDetection
TEMPLATE    = app

QT  += opengl

DEPENDPATH += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    cmainwindow.h \
    cqtopencvviewergl.h

SOURCES += \
    main.cpp \
    cmainwindow.cpp \
    cqtopencvviewergl.cpp

FORMS   += \
    cmainwindow.ui

PKGCONFIG   += opencv
CONFIG      += link_pkgconfig

#------------------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    message(Qt5 = $$QT)
}
else {
    error("Only Qt5")
}
#------------------------------------------------------------
VPATH = $$INCLUDEPATH
