QT       += testlib network
QT       -= gui

CONFIG += c++11 qt console warn_on depend_includepath
CONFIG -= app_bundle

INCLUDEPATH += $$PWD/../../src/ $$PWD

HEADERS += $$PWD/testrequesthandlerprovider.h \
    $$PWD/testrequesthandler.h \
    $$PWD/testnetworkreply.h  \
    $$PWD/tst_testbase.h

LIBS += -L$$OUT_PWD/../../../src/debug/ -liot
