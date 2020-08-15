
QT += 3dcore 3drender 3dinput 3dextras

DEPENDPATH  += $$PWD/src
INCLUDEPATH = $$DEPENDPATH

FOLDER  = tests

SOURCES += \
    main.cpp \
    orbittransformcontroller.cpp

HEADERS += \
    examples.pri \
    orbittransformcontroller.h

VPATH = $$INCLUDEPATH
