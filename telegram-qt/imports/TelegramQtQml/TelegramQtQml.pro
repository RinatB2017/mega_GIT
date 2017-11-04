QT = core network qml

TEMPLATE = lib
TARGET   = TelegramQt$${QT_MAJOR_VERSION}Qml
VERSION = 0.2.0

include(../../options.pri)

CONFIG += c++11

SOURCES = plugin.cpp
OTHER_FILES += CMakeLists.txt

target.path += $$INSTALL_QML_IMPORT_DIR/TelegramQt
INSTALLS += target

LIBS += -lssl -lcrypto -lz
LIBS += -L$$OUT_PWD/../../TelegramQt
LIBS += -lTelegramQt$${QT_MAJOR_VERSION}

qmlIndexFiles.files += qmldir plugins.qmltypes
qmlIndexFiles.path += $$target.path
INSTALLS += qmlIndexFiles
