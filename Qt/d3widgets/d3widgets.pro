#---
# https://habr.com/ru/post/218389/
#---

QT       += core gui webkit qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

contains(QT_VERSION, ^5.*) {
   QT += webkitwidgets
}

RCC_DIR = tmp/RCC
MOC_DIR = tmp/MOC
OBJECTS_DIR = tmp/OBJ
UI_DIR = tmp/UIC

TARGET = d3widgets
TEMPLATE = app


SOURCES += \
    main.cpp \
    base/d3viewer.cpp \
    base/d3webpage.cpp \
    charts/pie/dependencywheelwidget.cpp

HEADERS  += \
    base/d3viewer.h \
    base/d3webpage.h \
    charts/pie/dependencywheelwidget.h

RESOURCES += \
    resources/res.qrc
