#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

greaterThan(QT_MAJOR_VERSION, 4) {
    DEPENDPATH  += $$PWD/qt5/src
}
else {
    DEPENDPATH  += $$PWD/qt4/src
}
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

