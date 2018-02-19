TEMPLATE = app
TARGET = btfiletransfer

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/ui \
    $$PWD/images
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QT = core bluetooth widgets

SOURCES = \
    main.cpp \
    remoteselector.cpp \
    progress.cpp \
    pindisplay.cpp

HEADERS = \
    remoteselector.h \
    progress.h \
    pindisplay.h

FORMS = \
    remoteselector.ui \
    progress.ui \
    pindisplay.ui

OTHER_FILES += \
    busy.gif \
    pairing.gif

RESOURCES += \
    btfiletransfer.qrc
    
VPATH = $$INCLUDEPATH

