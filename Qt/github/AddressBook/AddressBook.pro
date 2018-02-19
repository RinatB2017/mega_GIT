
QT	   += core gui sql

PROGRAMM_PATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH += $$PROGRAMM_PATH
DEPENDPATH  += $$PROGRAMM_PATH

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AddressBook
TEMPLATE = app

SOURCES += \
    main.cpp \
    addressbookmainwindow.cpp \
    setupdialog.cpp \
    addressdialog.cpp \
    nationalitymodel.cpp \
    common/widgethelpers.cpp \
    common/basecombomodel.cpp \

HEADERS  += \
    addressbookmainwindow.h \
    setupdialog.h \
    addressdialog.h \
    nationalitymodel.h \
    common/widgethelpers.h \
    common/basecombomodel.h \

FORMS	+= \
    addressbookmainwindow.ui \
    setupdialog.ui \
    addressdialog.ui \

INCLUDEPATH += $$PWD/common

VPATH = $$INCLUDEPATH
