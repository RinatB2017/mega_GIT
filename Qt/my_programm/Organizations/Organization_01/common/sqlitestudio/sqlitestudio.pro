
TEMPLATE = app
TARGET = sqlitestudio

DEPENDPATH += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/ui \
    $$PWD/src/translations
INCLUDEPATH += src

QT += sql

DESTDIR = ../sqlitestudio

HEADERS += \
    src/aboutdialog.h \
    src/addcolumndialog.h \
    src/advancedsearchdialog.h \
    src/createindexdialog.h \
    src/createtabledialog.h \
    src/database.h \
    src/highlighter.h \
    src/importcsvdialog.h \
    src/logdialog.h \
    src/mainwindow.h \
    src/preferences.h \
    src/preferencesdialog.h \
    src/selectobjectdialog.h \
    src/sqlmodels.h \
    src/importcsvthread.h \
    src/createviewdialog.h

FORMS += \
    src/ui/aboutdialog.ui \
    src/ui/addcolumndialog.ui \
    src/ui/advancedsearchdialog.ui \
    src/ui/createindexdialog.ui \
    src/ui/createtabledialog.ui \
    src/ui/importcsvdialog.ui \
    src/ui/logdialog.ui \
    src/ui/mainwindow.ui \
    src/ui/preferencesdialog.ui \
    src/ui/selectobjectdialog.ui \
    src/ui/createviewdialog.ui

SOURCES += \
    src/aboutdialog.cpp \
    src/addcolumndialog.cpp \
    src/advancedsearchdialog.cpp \
    src/createindexdialog.cpp \
    src/createtabledialog.cpp \
    src/database.cpp \
    src/highlighter.cpp \
    src/importcsvdialog.cpp \
    src/logdialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/preferences.cpp \
    src/preferencesdialog.cpp \
    src/selectobjectdialog.cpp \
    src/sqlmodels.cpp \
    src/importcsvthread.cpp \
    src/createviewdialog.cpp

RESOURCES += src/resources.qrc
TRANSLATIONS += \
    languages/sqlitebrowser_fr.ts \
    languages/sqlitebrowser_ru.ts \
    languages/sqlitebrowser_en.ts

win32 {
    RC_FILE = winapp.rc
}

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc

greaterThan(QT_MAJOR_VERSION, 4) {
    message(Qt5)
    QT += widgets
}
