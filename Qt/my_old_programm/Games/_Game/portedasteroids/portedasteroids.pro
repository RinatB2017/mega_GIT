
TEMPLATE    = app
TARGET      = portedasteroids

PROGRAMM_PATH  += \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets
unix {
    OBJECTS_DIR = /dev/shm/my_programm/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$TARGET/rc
}


# Input
HEADERS += \
    ledmeter.h \
    sprites.h \
    toplevel.h \
    view.h \
    animateditem.h

SOURCES += \
    ledmeter.cpp \
    main.cpp \
    toplevel.cpp \
    view.cpp \
    animateditem.cpp

RESOURCES += portedasteroids.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/graphicsview/portedasteroids
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS portedasteroids.pro bg.png sounds sprites
sources.path = $$[QT_INSTALL_EXAMPLES]/graphicsview/portedasteroids

INSTALLS += target sources

symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)

simulator: warning(This example might not fully work on Simulator platform)

VPATH = $$INCLUDEPATH
