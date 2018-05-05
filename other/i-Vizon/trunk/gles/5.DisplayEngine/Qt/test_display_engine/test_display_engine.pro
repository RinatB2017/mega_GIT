include(../../../wrapper/glesWrapper.pri)
include(../../../display_engine/displayEngine.pri)
include(../../../../opencv/wrapper/cvWrapper.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += ../../test_display_engine_main.cpp

