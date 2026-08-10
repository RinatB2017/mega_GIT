
DEPENDPATH  += $$PWD
INCLUDEPATH = $$DEPENDPATH

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

QT += sql

HEADERS += \
    aboutdialog.h \
    addcolumndialog.h \
    advancedsearchdialog.h \
    createindexdialog.h \
    createtabledialog.h \
    database.h \
    highlighter.h \
    importcsvdialog.h \
    logdialog.h \
    mainwindow.h \
    preferences.h \
    preferencesdialog.h \
    selectobjectdialog.h \
    sqlmodels.h \
    importcsvthread.h \
    createviewdialog.h

FORMS += \
    aboutdialog.ui \
    addcolumndialog.ui \
    advancedsearchdialog.ui \
    createindexdialog.ui \
    createtabledialog.ui \
    importcsvdialog.ui \
    logdialog.ui \
    mainwindow.ui \
    preferencesdialog.ui \
    selectobjectdialog.ui \
    createviewdialog.ui

SOURCES += \
    aboutdialog.cpp \
    addcolumndialog.cpp \
    advancedsearchdialog.cpp \
    createindexdialog.cpp \
    createtabledialog.cpp \
    database.cpp \
    highlighter.cpp \
    importcsvdialog.cpp \
    logdialog.cpp \
    mainwindow.cpp \
    preferences.cpp \
    preferencesdialog.cpp \
    selectobjectdialog.cpp \
    sqlmodels.cpp \
    importcsvthread.cpp \
    createviewdialog.cpp

RESOURCES += resources.qrc

TRANSLATIONS += \
    languages/sqlitebrowser_fr.ts \
    languages/sqlitebrowser_ru.ts \
    languages/sqlitebrowser_en.ts
