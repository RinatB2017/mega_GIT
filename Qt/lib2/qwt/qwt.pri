#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

CONFIG += qwt
DEFINES += QT_DLL QWT_DLL
QT   += svg opengl printsupport concurrent

unix {
    DEPENDPATH  += /usr/include/qwt6/
    INCLUDEPATH = $$DEPENDPATH
    greaterThan(QT_MAJOR_VERSION, 4) {
        LIBS += -lqwt6-qt5
    }
    else {
        LIBS += -lqwt6-qt4
    }
}

win32 {
#    QWT_PATH = "qwt-614"
#    QWT_PRI  = "qwt-614.pri"

#    QWT_PATH = "qwt-615"
#    QWT_PRI  = "qwt-615.pri"

    QWT_PATH = "qwt-620"
    QWT_PRI  = "qwt-620.pri"

    DEPENDPATH  += $$PWD/$$QWT_PATH/src/
    INCLUDEPATH += $$PWD/$$QWT_PATH/src/

    LIBS    += -L$$PWD/lib
    LIBS    += -lqwt
}
