TEMPLATE = app

QT += qml quick widgets network

CONFIG += c++11

SOURCES += main.cpp buslistmodel.cpp \
    bus.cpp

HEADERS += buslistmodel.h \
    bus.h

RESOURCES += qml.qrc

INCLUDEPATH += $$PWD/../../src/ $$PWD
LIBS += -L$$OUT_PWD/../../src/debug/ -liot

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
