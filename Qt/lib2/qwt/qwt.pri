#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

unix {
    DEPENDPATH  += /usr/include/qwt6/
    INCLUDEPATH = $$DEPENDPATH
    greaterThan(QT_MAJOR_VERSION, 4) {
        LIBS += -lqwt6-qt5
    }
    else {
        LIBS += -lqwt6-qt4
    }
    QT   *= svg opengl printsupport concurrent

    CONFIG += qwt
}

win32 {
#     QWT_PATH = "qwt-613"
#     QWT_PRI  = "qwt-613.pri"

#     QWT_PATH = "qwt-614"
#     QWT_PRI  = "qwt-614.pri"

    QWT_PATH = "qwt-615"
    QWT_PRI  = "qwt-615.pri"

    DEPENDPATH  += $$PWD/$$QWT_PATH/src/
    INCLUDEPATH = $$DEPENDPATH
    include ($$PWD/$$QWT_PATH/$$QWT_PRI)

    CONFIG += qwt
}
