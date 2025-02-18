QT       += core network websockets gui widgets

TEMPLATE = app

TARGET = MoolticuteApp

CONFIG += c++11

mac {
    LIBS += -framework ApplicationServices -framework IOKit -framework CoreFoundation -framework Cocoa -framework Foundation
}
win32 {
    LIBS += -luser32
}

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$TARGET/rc
}

include (src/QtAwesome/QtAwesome/QtAwesome.pri)
include (src/QSimpleUpdater/QSimpleUpdater.pri)

SOURCES += \
    src/main_gui.cpp \
    src/MainWindow.cpp \
    src/Common.cpp \
    src/WSClient.cpp \
    src/RotateSpinner.cpp \
    src/CredentialsModel.cpp \
    src/AppGui.cpp \
    src/DaemonMenuAction.cpp \
    src/AutoStartup.cpp \
    src/WindowLog.cpp \
    src/OutputLog.cpp \
    src/AnsiEscapeCodeHandler.cpp \
    src/PasswordLineEdit.cpp \
    src/CredentialsView.cpp \
    src/CredentialsManagement.cpp \
    src/zxcvbn-c/zxcvbn.c \
    src/FilesManagement.cpp \
    src/SSHManagement.cpp

HEADERS  += \
    src/MainWindow.h \
    src/Common.h \
    src/QtHelper.h \
    src/WSClient.h \
    src/RotateSpinner.h \
    src/CredentialsModel.h \
    src/version.h \
    src/AppGui.h \
    src/DaemonMenuAction.h \
    src/AutoStartup.h \
    src/WindowLog.h \
    src/OutputLog.h \
    src/AnsiEscapeCodeHandler.h \
    src/PasswordLineEdit.h \
    src/CredentialsView.h \
    src/CredentialsManagement.h \
    src/zxcvbn-c/dict-src.h \
    src/zxcvbn-c/zxcvbn.h \
    src/FilesManagement.h \
    src/SSHManagement.h

mac {
    HEADERS += src/MacUtils.h
    OBJECTIVE_SOURCES += src/MacUtils.mm
}

INCLUDEPATH += src\
    src/zxcvbn-c

FORMS    += \
    src/MainWindow.ui \
    src/WindowLog.ui \
    src/CredentialsManagement.ui \
    src/FilesManagement.ui \
    src/SSHManagement.ui

RESOURCES += \
    img/images.qrc

win32 {
    RC_FILE = win/windows_res.rc
}

mac {
    ICON = img/AppIcon.icns
} else {
    ICON = img/AppIcon.svg
}

unix {
    # INSTALL RULES
    #
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    DEFINES += MC_INSTALL_PREFIX=\\\"$$PREFIX\\\"

    # install the binary
    target.path = $$PREFIX/bin
    INSTALLS += target

    # install the desktop files
    xdgdesktop.path = $$PREFIX/share/applications
    xdgdesktop.files += $$PWD/linux/moolticute.desktop
    INSTALLS += xdgdesktop

    # install icon
    ico.path = $$PREFIX/share/icons
    ico.files += $$PWD/linux/moolticute.png
    INSTALLS += ico
}

