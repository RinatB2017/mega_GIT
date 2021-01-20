HEADERS       = client.h
SOURCES       = client.cpp \
                main.cpp
QT           += network widgets

VER_MAJOR = 1
VER_MINOR = 0
VER_PATCH = 0
VER_BUILD = 0

DEFINES += VER_MAJOR=$${VER_MAJOR}
DEFINES += VER_MINOR=$${VER_MINOR}
DEFINES += VER_BUILD=$${VER_BUILD}
DEFINES += VER_PATCH=$${VER_PATCH}

win32 {
    VERSION = $${VER_MAJOR}"."$${VER_MINOR}"."$${VER_PATCH}"."$${VER_BUILD}

    QMAKE_TARGET_COMPANY = Home
    QMAKE_TARGET_PRODUCT = $$TARGET
    QMAKE_TARGET_COPYRIGHT = "Copyright \\251 2020-2025"
    QMAKE_TARGET_DESCRIPTION = "my description"

    RC_ICONS = "ico/computer.ico"
    # RC_FILE  = programm.rc
}

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/ipc/localfortuneclient
INSTALLS += target
