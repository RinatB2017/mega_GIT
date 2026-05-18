#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += $$PWD/qt6/src
#     DEPENDPATH  += $$PWD/qt5/src
#     DEPENDPATH  += $$PWD/qt4/src

INCLUDEPATH = $$DEPENDPATH

SOURCES += \
    qtpropertybrowser.cpp \
    qtpropertymanager.cpp \
    qteditorfactory.cpp \
    qtvariantproperty.cpp \
    qttreepropertybrowser.cpp \
    qtbuttonpropertybrowser.cpp \
    qtgroupboxpropertybrowser.cpp \
    qtpropertybrowserutils.cpp

HEADERS += \
    qtpropertybrowser.h \
    qtpropertymanager.h \
    qteditorfactory.h \
    qtvariantproperty.h \
    qttreepropertybrowser.h \
    qtbuttonpropertybrowser.h \
    qtgroupboxpropertybrowser.h \
    qtpropertybrowserutils_p.h

