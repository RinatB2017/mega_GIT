
TEMPLATE    = app
TARGET      = KMines

DEPENDPATH  += \
    /usr/include/KF5/KDELibs4Support \
    /usr/include/KF5/KTextWidgets \
    /usr/include/KF5/KConfigCore \
    /usr/include/KF5/KDBusAddons \
    /usr/include/KF5/KCrash \
    /usr/include/KF5/KConfigWidgets \
    /usr/include/KF5/KI18n \
    /usr/include/KF5/KWidgetsAddons \
    /usr/include/KF5/KCoreAddons \
    /usr/include/KF5/KF5KDEGames/KDE \
    /usr/include/KF5/KXmlGui \
    /usr/include/KF5/KF5KDEGames \
    /usr/include/KF5

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT      += qml
QT      -= core gui
LIBS    += -lkdegames

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    message(Qt5 = $$QT)
}
else {
    message(Qt4 = $$QT)
}

HEADERS += \
    src/borderitem.h \
    src/cellitem.h \
    src/commondefs.h \
    src/mainwindow.h \
    src/minefielditem.h \
    src/scene.h \
    src/version.h \
    src/settings.h

SOURCES += \
    src/borderitem.cpp \
    src/cellitem.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/minefielditem.cpp \
    src/scene.cpp \
    src/settings.cpp

FORMS += \
    src/ui/customgame.ui \
    src/ui/generalopts.ui

VPATH = $$INCLUDEPATH
