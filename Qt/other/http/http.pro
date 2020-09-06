
QT += network widgets

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += \
    httpwindow.h

SOURCES += \
    httpwindow.cpp \
    main.cpp

FORMS += \
    authenticationdialog.ui

VPATH = $$INCLUDEPATH
