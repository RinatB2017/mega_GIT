
DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

QT          += widgets

HEADERS     = \
    treeitem.h \
    treemodel.h

RESOURCES   = \
    simpletreemodel.qrc

SOURCES     = \
    treeitem.cpp \
    treemodel.cpp \
    main.cpp

VPATH = $$INCLUDEPATH
