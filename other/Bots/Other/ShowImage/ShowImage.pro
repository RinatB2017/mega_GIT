
TARGET      = ShowImage
TEMPLATE    = app

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS  += \
    mainwindow.h

SOURCES += \
    mainwindow.cpp \
    main.cpp

FORMS    += mainwindow.ui

PKGCONFIG   += opencv
CONFIG      += link_pkgconfig

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}
else {
    error ("Only Qt5")
}

VPATH = $$INCLUDEPATH
