#-------------------------------------------------
#
# Project created by QtCreator 2014-06-28T11:45:37
#
#-------------------------------------------------

QT += widgets webkitwidgets network

TARGET = dooscape

FOLDER  = old_programm

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/other
INCLUDEPATH = $$DEPENDPATH

win32 {
    QT += winextras
    TARGET = Dooscape
}

TEMPLATE = app

TRANSLATIONS = dooscape_fr.ts\

RC_FILE = res.rc

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    sidepanel.cpp \
    webview.cpp \
    webinfo.cpp \
    progressindicator.cpp \
    commandbar.cpp \
    panel.cpp \
    tabview.cpp \
    aboutsoftware.cpp \
    progressview.cpp \
    settingswindow.cpp \
    starteruser.cpp \
    lockscreen.cpp \
    screenshoot.cpp \
    bookmarksmanager.cpp \
    scapeui.cpp

HEADERS  += \
    mainwindow.h \
    sidepanel.h \
    webview.h \
    webinfo.h \
    progressindicator.h \
    commandbar.h \
    panel.h \
    tabview.h \
    aboutsoftware.h \
    progressview.h \
    settingswindow.h \
    starteruser.h \
    lockscreen.h \
    screenshoot.h \
    bookmarksmanager.h \
    scapeui.h

RESOURCES += \
    res.qrc

OTHER_FILES += \
    res.rc \
    res/html.css

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$TARGET/rc

    CONFIG(debug,debug|release) {
    #    DESTDIR = bin
        DESTDIR = $$(HOME)/Programming/my_programm_bin/$$FOLDER/$$TARGET
    } else {
    #    DESTDIR = bin
        DESTDIR = $$(HOME)/Programming/my_programm_bin/$$FOLDER/$$TARGET
    }
}

win32 {
    TEMP_PATH = "C:\\temp"
    OBJECTS_DIR = $$TEMP_PATH\\my_programm\\$$TARGET\\obj
    MOC_DIR     = $$TEMP_PATH\\my_programm\\$$TARGET\\moc
    UI_DIR      = $$TEMP_PATH\\my_programm\\$$TARGET\\ui
    RCC_DIR     = $$TEMP_PATH\\my_programm\\$$TARGET\\rc

    DESTDIR = bin
}

QMAKE_CFLAGS    += -std=c++11
QMAKE_CXXFLAGS  += -std=c++11
QMAKE_LFLAGS    += -std=c++11

VPATH = $$INCLUDEPATH

