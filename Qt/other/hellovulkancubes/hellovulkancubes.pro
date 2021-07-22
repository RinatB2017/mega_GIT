
PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

QT += widgets concurrent

HEADERS += \
    mainwindow.h \
    vulkanwindow.h \
    renderer.h \
    mesh.h \
    shader.h \
    camera.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    vulkanwindow.cpp \
    renderer.cpp \
    mesh.cpp \
    shader.cpp \
    camera.cpp

RESOURCES += \
    images/hellovulkancubes.qrc

VPATH = $$INCLUDEPATH
