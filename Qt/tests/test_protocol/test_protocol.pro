#-------------------------------------------------
#
# Project created by QtCreator 2018-06-19T14:56:39
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET = test_protocol
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    src/tst_protocol.cpp

include (src/base_protocol/base_protocol.pri)
include (src/crc/crc.pri)

include (src/test/test.pri)

VPATH = $$INCLUDEPATH
