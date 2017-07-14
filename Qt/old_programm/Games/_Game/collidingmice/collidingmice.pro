
TEMPLATE    = app
TARGET      = collidingmice


HEADERS += \
	mouse.h
SOURCES += \
	main.cpp \
        mouse.cpp

RESOURCES += \
	mice.qrc
	
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

# install
target.path = $$[QT_INSTALL_EXAMPLES]/graphicsview/collidingmice
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS collidingmice.pro images
sources.path = $$[QT_INSTALL_EXAMPLES]/graphicsview/collidingmice
INSTALLS += target sources

symbian {
    TARGET.UID3 = 0xA000A643
    include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
}
maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)
simulator: warning(This example might not fully work on Simulator platform)

VPATH = $$INCLUDEPATH
