
DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS     = \
    glwidget.h \
    window.h \
    mainwindow.h \
    logo.h
    
SOURCES     = \
    glwidget.cpp \
    main.cpp \
    window.cpp \
    mainwindow.cpp \
    logo.cpp

greaterThan(QT_MAJOR_VERSION, 4) {
    QT  += widgets
    message(Qt5 = $$QT)
}
else {
    message(Qt4 = $$QT)
}

VPATH = $$INCLUDEPATH

