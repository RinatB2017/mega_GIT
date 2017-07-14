QT += widgets
qtHaveModule(printsupport): QT += printsupport

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH


HEADERS += \
    imageviewer.h

SOURCES += \
    imageviewer.cpp \
    main.cpp

VPATH = $$INCLUDEPATH
