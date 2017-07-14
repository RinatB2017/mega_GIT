#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

DEPENDPATH  += \
    $$PWD \
    $$PWD/src \
    $$PWD/src/ui
INCLUDEPATH = $$DEPENDPATH

HEADERS += aboutbox.hpp
SOURCES += aboutbox.cpp

contains(DEFINES, LOGO_GL) {
    message (LOGO_GL)
    DEPENDPATH  += glwidget
    INCLUDEPATH = $$DEPENDPATH
    HEADERS += \
        glwidget.h \
        qtlogo.h
    SOURCES += \
        glwidget.cpp \
        qtlogo.cpp
    QT *= opengl
}

FORMS       += aboutbox.ui

RESOURCES   += aboutbox.qrc
