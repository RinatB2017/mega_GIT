
DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT += network

SOURCES += \
    qtsingleapplication.cpp \
    qtlocalpeer.cpp

HEADERS += \
    qtsingleapplication.h \
    qtlocalpeer.h
