#-------------------------------------------------
#
# Project created by QtCreator 2015-05-21T10:16:49
#
#-------------------------------------------------

QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = bluetooth
TEMPLATE    = app

PROGRAMM_PATH  += \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS  += \
    mainwindow.h

FORMS    += \
    mainwindow.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    images/btrfcomm.qrc

DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android


VPATH = $$INCLUDEPATH
