include(../../../wrapper/glesWrapper.pri)
include(../../../../opencv/wrapper/cvWrapper.pri)


####################################
# Project Section                  #
####################################

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += ../../test_wrapper.cpp

HEADERS += ../../banana.h

