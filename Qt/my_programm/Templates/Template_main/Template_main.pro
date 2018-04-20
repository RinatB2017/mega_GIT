#-------------------------------------------------
#
# Project created by QtCreator 2017-04-27T22:37:49
#
#-------------------------------------------------

TARGET      = Template_main
TEMPLATE    = app

QT       += core gui

unix:!macx {
    OBJECTS_DIR = /dev/shm/my_programm/$$FOLDER/$$TARGET/obj
    MOC_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/moc
    UI_DIR      = /dev/shm/my_programm/$$FOLDER/$$TARGET/ui
    RCC_DIR     = /dev/shm/my_programm/$$FOLDER/$$TARGET/rc
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

HEADERS += src/defines.hpp
SOURCES += src/main.cpp

RESOURCES += \
    images/images.qrc

include (mainwindow/mainwindow.pri)
include (mainwidget/mainwidget.pri)
include (menudock/menudock.pri)
include (turbo/turbo.pri)

include (logdock/logdock.pri)
include (log_options/log_options.pri)
include (log/log.pri)

include (common/common.pri)
include (other/other.pri)

VPATH = $$INCLUDEPATH
