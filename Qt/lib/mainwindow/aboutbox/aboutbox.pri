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

greaterThan(QT_MAJOR_VERSION, 4) {
    include (glwidget/glwidget_qt5.pri)
} else {
    include (glwidget/glwidget_qt4.pri)
}

    QT *= opengl
}

FORMS       += aboutbox.ui

RESOURCES   += aboutbox.qrc
