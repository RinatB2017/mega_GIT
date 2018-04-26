#-------------------------------------------------
#
# Project created by QtCreator 2014-04-16T18:44:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_gui_inputs
TEMPLATE = app


SOURCES += main.cpp \
    recorderSettings.cpp   \
    msg_q/src/msg_q_linux_wrapper.cpp

HEADERS  += \
    recorderSettings.h\
    msg_q/inc/msg_q_linux_wrapper.h

FORMS    += \
    recordersettings.ui
