
DEPENDPATH  += \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH


HEADERS     = \
    domitem.h \
    dommodel.h \
    mainwindow.h
    
SOURCES     = \
    domitem.cpp \
    dommodel.cpp \
    main.cpp \
    mainwindow.cpp
    
QT      += xml widgets

VPATH = $$INCLUDEPATH
