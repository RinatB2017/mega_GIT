TEMPLATE = app
TARGET = audiooutput

QT += multimedia widgets

DEPENDPATH  += \
    $$PWD \
    $$PWD/src
INCLUDEPATH = $$DEPENDPATH

HEADERS       = audiooutput.h

SOURCES       = audiooutput.cpp \
                main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/audiooutput
INSTALLS += target

VPATH = $$INCLUDEPATH
