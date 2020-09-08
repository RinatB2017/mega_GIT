#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD/src \
    $$PWD/src/ui \
    $$PWD/images
INCLUDEPATH = $$DEPENDPATH

CONFIG += designer

HEADERS += \
    properties_dialog.h \
    qmeter.h \
    qmetertaskmenu.h

SOURCES += \
    properties_dialog.cpp \
    qmeter.cpp \
    qmeterdrawfunctions.cpp \
    qmetertaskmenu.cpp

FORMS += \
    properties.ui \
    test.ui
           
RESOURCES += qmeter.qrc
