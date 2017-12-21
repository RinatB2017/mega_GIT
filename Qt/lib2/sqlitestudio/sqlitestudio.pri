
DEPENDPATH  += $$PWD
INCLUDEPATH = $$DEPENDPATH

QT += sql

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
    src/aboutdialog.ui \
    src/addcolumndialog.ui \
    src/advancedsearchdialog.ui \
    src/createindexdialog.ui \
    src/createtabledialog.ui \
    src/importcsvdialog.ui \
    src/logdialog.ui \
    src/mainwindow.ui \
    src/preferencesdialog.ui \
    src/selectobjectdialog.ui \
    src/createviewdialog.ui

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
